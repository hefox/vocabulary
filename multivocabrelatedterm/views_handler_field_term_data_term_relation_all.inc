<?php
// $Id: views_handler_field_term_node_tid.inc,v 1.4 2009/07/01 23:07:14 merlinofchaos Exp $

/**
 * Field handler for terms.
 */
class views_handler_field_term_data_term_relation_all extends views_handler_field_prerender_list {

  function option_definition() {
    $options = parent::option_definition();

    $options['link_to_taxonomy'] = array('default' => TRUE);
    $options['limit'] = array('default' => FALSE);
    $options['vids'] = array('default' => array());

    return $options;
  }

  /**
   * Provide "link to term" option.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['link_to_taxonomy'] = array(
      '#title' => t('Link this field to its term page'),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['link_to_taxonomy']),
    );

    $form['limit'] = array(
      '#type' => 'checkbox',
      '#title' => t('Limit terms by vocabulary'),
      '#default_value'=> $this->options['limit'],
    );

    $options = array();
    $vocabularies = taxonomy_get_vocabularies();
    foreach ($vocabularies as $voc) {
      $options[$voc->vid] = check_plain($voc->name);
    }

    $form['vids'] = array(
      '#prefix' => '<div><div id="edit-options-vids">',
      '#suffix' => '</div></div>',
      '#type' => 'checkboxes',
      '#title' => t('Vocabularies'),
      '#options' => $options,
      '#default_value' => $this->options['vids'],
      '#process' => array('expand_checkboxes', 'views_process_dependency'),
      '#dependency' => array('edit-options-limit' => array(TRUE)),
    );
  }
  function pre_render($values) {
    $vids = array();
    foreach ($values as $result) {
      if (!empty($result->tid)) {
        $tids[] = $result->tid;
      }
    }
    
    if ($tids) {
      $voc = '';
      if (!empty($this->options['limit']) && !empty($this->options['vids'])) {
        $voc = " AND td.vid IN (" . implode(', ', array_keys(array_filter($this->options['vids']))) . ")";
      }
      foreach($tids as $tid) {
        foreach (taxonomy_get_related($tid) as $term) {
          if ($this->options['vids'] && $this->options['limit'] && !in_array($term->vid,$this->options['vids'])) continue;
          $this->items[$tid][$term->tid]['name'] = check_plain($term->name);
          $this->items[$tid][$term->tid]['tid'] = $term->tid;
          $this->items[$tid][$term->tid]['vid'] = $term->vid;
         // $this->items[$tid][$term->tid]['vocabulary'] = check_plain($term->vocabulary);
          //too lazy to fix atm
          if (!empty($this->options['link_to_taxonomy'])) {
            $this->items[$tid][$term->tid]['make_link'] = TRUE;
            $this->items[$tid][$term->tid]['path'] = taxonomy_term_path($term);
          }
        }
      }
    }
  }

  function render_item($count, $item) {
    return $item['name'];
  }

  function document_self_tokens(&$tokens) {
    $tokens['[' . $this->options['id'] . '-tid' . ']'] = t('The taxonomy term ID for the term.');
    $tokens['[' . $this->options['id'] . '-name' . ']'] = t('The taxonomy term name for the term.');
    $tokens['[' . $this->options['id'] . '-vid' . ']'] = t('The vocabulary ID for the vocabulary the term belongs to.');
    $tokens['[' . $this->options['id'] . '-vocabulary' . ']'] = t('The name for the vocabulary the term belongs to.');
  }

  function add_self_tokens(&$tokens, $item) {
    $tokens['[' . $this->options['id'] . '-tid' . ']'] = $item['tid'];
    $tokens['[' . $this->options['id'] . '-name' . ']'] = $item['name'];
    $tokens['[' . $this->options['id'] . '-vid' . ']'] = $item['vid'];
    $tokens['[' . $this->options['id'] . '-vocabulary' . ']'] = $item['vocabulary'];
  }
}


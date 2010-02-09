<?php

function vocabulary_page($vocabulary) {

    $query = db_select('taxonomy_term_data', 't')->extend('PagerDefault');
    $query->addTag('translatable');
    $query->addTag('term_access');
    $tags_return = $query
      ->fields('t', array('tid', 'name'))
      ->condition('t.vid', $vocabulary->vid)
      ->orderBy('weight', 'ASC')
      ->limit(variable_get('default_terms_main', 10))
      ->execute()
      ->fetchAllKeyed();
  if($tags_return){
    $terms = taxonomy_term_load_multiple(array_keys($tags_return));
    $build = taxonomy_term_view_multiple($terms);
    $build['pager'] = array(
      '#theme' => 'pager',
      '#weight' => 5,
    );
  } else {
    $build['no_content'] = array(
      '#prefix' => '<p>',
      '#markup' => t('There is currently no content classified with this term.'),
      '#suffix' => '</p>',
    );
  }

  return $build;
}


function taxonomy_term_view_multiple($terms, $view_mode = 'teaser', $weight = 0) {
  field_attach_prepare_view('taxonomy_term', $terms, $view_mode);
  entity_prepare_view('taxonomy_term', $terms);
  $build = array();
  foreach ($terms as $term) {
    $build['terms'][$term->tid] = taxonomy_term_view($term, $view_mode);
    $build['terms'][$term->tid]['#weight'] = $weight;
    $weight++;
  }
  $build['terms']['#sorted'] = TRUE;
  return $build;
}

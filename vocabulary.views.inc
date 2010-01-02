<?php

function vocabulary_views_data() {
  $data = array();
    $data['term_node']['all_tid'] = array(
    'title' => t('Term ID-- multiple vocabularies'),
    'help' => t('The taxonomy term ID'),
        'real field'=> 'tid',
    'filter' => array(
      'title' => t('Term - multiple vocabularies'),
      'handler' => 'views_handler_filter_term_node_all_tid',
      'hierarchy table' => 'term_hierarchy',
      'numeric' => TRUE,
      'skip base' => 'term_data',
      'allow empty' => TRUE,
    ),
  );
  return $data;
}

function vocabulary_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'vocabulary'),
    ),
    'handlers' => array(
        'views_handler_filter_term_node_all_tid' => array(
                'parent' => 'views_handler_filter_many_to_one',
       ),
    )
  );
}

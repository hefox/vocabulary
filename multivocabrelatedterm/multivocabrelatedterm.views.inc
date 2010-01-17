<?php

function multivocabrelatedterm_views_data() {
  $data = array();
  $data['term_data']['term_relationship'] = array(
    'group' => t('Taxonomy'),
    'title' => t('Term relation'),
    'help' => t('Related term.'),
    'real field' => 'tid',
    'argument' => array(
      'handler' => 'views_handler_argument_term_data_term_relationship',
    ),
  );
  $data['term_data']['term_relation_all'] = array(
    'group' => t('Related Terms'),
    'title' => t('Related terms made via term_relation'),
    'help' => t('Related term.'),
    'real field' => 'tid',
    'field' => array(
      'handler' => 'views_handler_field_term_data_term_relation_all',
    ),
  );
  return $data;
}

function multivocabrelatedterm_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'multivocabrelatedterm'),
    ),
    'handlers' => array(
      'views_handler_argument_term_data_term_relationship' => array(
        'parent' => 'views_handler_argument',
      ),
      'views_handler_field_term_data_term_relation_all' => array(
        'parent' => 'views_handler_field_prerender_list',
      ),
    ),
  );
}

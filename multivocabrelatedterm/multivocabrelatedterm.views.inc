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
    ),
  );
}

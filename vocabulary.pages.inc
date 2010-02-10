<?php

function vocabulary_page($vocabulary) {
    $build = taxonomy_vocabulary_view($vocabulary);
    
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
    $build = array_merge($build,taxonomy_term_view_multiple($terms));
    $build['pager'] = array(
      '#theme' => 'pager',
      '#weight' => 5,
    );
  } else {
    $build['no_content'] = array(
      '#prefix' => '<p>',
      '#markup' => t('There is currently no content terms for this vocabulary.'),
      '#suffix' => '</p>',
    );
  }

  return $build;
}



<?php

/**
* Implementation of hook_taxonomy_menu_path.
*
* @return array
*  function name => Display Title
*  a list of the path options.
*/
function vocabulary_taxonomy_menu_path() {
  $output = array('vocabulary_path_vocab_view' => t('Vocabulary module path'));
  return $output;
}

/**
* Callback for hook_taxonomy_menu_path
*/
function vocabulary_path_vocab_view($vid, $tid) {
  if ($tid == 0) { 
    return 'taxonomy/'.$vid;
  } else {
    return taxonomy_menu_path_default($vid, $tid);
  }
}

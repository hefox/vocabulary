<?php

/*
Below all the junk to associate the page with ctools
*/
function vocabulary_vocabulary_view_page_manager_tasks() {
  $plugins['vocabulary_view'] = array(
    // This is a 'page' task and will fall under the page admin UI
    'task type' => 'page',
    'title' => t('Vocabulary template'),
    'admin title' => t('Vocabulary template'),
    'admin description' => t('When enabled, this overrides the default vocabulary module behavior for displaying vocabulary at <em>taxonomy/%taxonomy_vocabulary</em>.'),
    'admin path' => 'taxonomy/%taxonomy_vocabulary',
    'admin summary' => 'vocabulary_admin_summary',
  
    // Menu hooks so that we can alter the term/%term menu entry to point to us.
    'hook menu' => 'vocabulary_menu',
    'hook menu alter' => 'page_manager_vocabulary_menu_alter',
  
    // Provide a setting to the primary settings UI for Panels
    'admin settings' => 'vocabulary_admin_settings',
    // Even though we don't have subtasks, this allows us to save our settings.
    'save subtask callback' => 'vocabulary_save',
  
    // Callback to add items to the page managertask administration form:
    'task admin' => 'vocabulary_task_admin',
  
    // This is task uses 'context' handlers and must implement these to give the
    // handler data it needs.
    'handler type' => 'context',
    'get arguments' => 'vocabulary_get_arguments',
    'get context placeholders' => 'vocabulary_get_contexts',
  
    // Allow additional operations
    'operations' => array(
      'settings' => array(
        'title' => t('Settings'),
        'description' => t('Update settings specific to the Vocabulary view.'),
      ),
      // This lets it automatically add relevant information for task handlers.
      'handlers' => array('type' => 'handlers'),
    ),
  
    // Allow this to be enabled or disabled:
    'disabled' => variable_get('vocabulary_disabled', TRUE),
    'enable callback' => 'page_manager_vocabulary_enable',
  
    'operations' => array(
      'settings' => array(
        'type' => 'group',
        'class' => 'operations-settings',
        'title' => t('Settings'),
        'children' => array(
          'basic' => array(
            'title' => t('Basic'),
            'description' => t('Edit name, path and other basic settings for the page.'),
            'form' => 'vocabulary_settings',
          ),
        ),
      ),
    ),
  );
  return $plugins;
}  
  
/**
 * Callback defined by vocabulary_page_manager_tasks().
 *
 * Alter the term view input so that term view comes to us rather than the
 * normal term view process.
 */
function page_manager_vocabulary_menu_alter(&$items, $task) {
  if (variable_get('vocabulary_disabled', TRUE)) {
    return;
  }
  // Override the term view handler for our purpose, but only if someone else
  // has not already done so.
  if (isset($items['taxonomy/%taxonomy_vocabulary']) && $items['taxonomy/%taxonomy_vocabulary']['page callback'] == 'vocabulary_page' || variable_get('page_manager_override_anyway', FALSE)) {
    $items['taxonomy/%taxonomy_vocabulary']['page callback'] = 'page_manager_vocabulary';
    $items['taxonomy/%taxonomy_vocabulary']['file path'] = $task['path'];
    $items['taxonomy/%taxonomy_vocabulary']['file'] = $task['file'];
  }
  else {
    // automatically disable this task if it cannot be enabled.
    variable_set('vocabulary_disabled', TRUE);
    if (!empty($GLOBALS['page_manager_enabling_vocabulary'])) {
      drupal_set_message(t('Page manager module is unable to enable taxonomy/%taxonomy_vocabulary because some other module already has overridden with %callback.', array('%callback' => $items['taxonomy/%taxonomy_vocabulary']['page callback'])), 'error');
    }
  }
}

/**
 * Entry point for our overridden term view.
 *
 * This function asks its assigned handlers who, if anyone, would like
 * to run with it. If no one does, it passes through to Drupal core's
 * term view, which is term_page_view().
 */
function page_manager_vocabulary($vocabulary, $op = 'page') {
  // While we ordinarily should never actually get feeds through here,
  // just in case
  if ($op != 'feed') {
    // Load my task plugin
    $task = page_manager_get_task('vocabulary_view');
    
    // Load the term into a context.
    ctools_include('context');
    ctools_include('context-task-handler');
    
    $contexts = ctools_context_handler_get_task_contexts($task, '', array($vocabulary->vid));

    if (empty($contexts)) {
      return drupal_not_found();
    }

    // Add a fake tab for 'View' so that edit tabs can be added.
    if (user_access('administer page manager')) {
      ctools_include('menu');
      ctools_menu_add_tab(array(
        'title' => t('View'),
        'href' => $_GET['q'],
        'type' => MENU_DEFAULT_LOCAL_TASK,
        'weight' => -10,
      ));
    }
    $output = ctools_context_handler_render($task, '', $contexts, array($vocabulary->vid, $op));
    if ($output !== FALSE) {
      return $output;
    }
  }

  // Otherwise, fall back.
  return vocabulary_page($vocabulary, $op);
}

/**
 * Callback to get arguments provided by this task handler.
 *
 * Since this is the term view and there is no UI on the arguments, we
 * create dummy arguments that contain the needed data.
 */
function vocabulary_get_arguments($task, $subtask_id) {
  return array(
    array(
      'keyword' => 'vocabulary',
      'identifier' => t('Vocabulary being viewed'),
      'id' => 1,
      'name' => 'vid',
      'settings' => array('input_form' => 'vid'),
      'default' => '404',
    ),
  );
}

/**
 * Callback to get context placeholders provided by this handler.
 */
function vocabulary_get_contexts($task, $subtask_id) {
  return ctools_context_get_placeholders_from_argument(vocabulary_get_arguments($task, $subtask_id));
}

/**
 * Settings page for this item.
 */
function vocabulary_settings(&$form, &$form_state) {
  // This passes thru because the setting can also appear on the main Panels
  // settings form. If $settings is an array it will just pick up the default.
  $settings = isset($form_state->update_values) ? $form_state->update_values : array();
  vocabulary_admin_settings($form, $settings);
}

/**
 * Copy form values into the page cache.
 */
function vocabulary_settings_submit(&$form, &$form_state) {
  $form_state['page']->update_values = $form_state['values'];
}

/**
 * Save when the page cache is saved.
 */
function vocabulary_save($subtask, $cache) {
}

/**
 * Provide a setting to the Panels administrative form.
 */
function vocabulary_admin_settings(&$form, $settings = array()) {
  return array(); 
}



/**
 * Callback to enable/disable the page from the UI.
 */
function page_manager_vocabulary_enable($cache, $status) {
  variable_set('vocabulary_disabled', $status);

  // Set a global flag so that the menu routine knows it needs
  // to set a message if enabling cannot be done.
  if (!$status) {
    $GLOBALS['page_manager_enabling_vocabulary'] = TRUE;
  }
}

/**
 * Provide a nice administrative summary of the page so an admin can see at a
 * glance what this page does and how it is configured.
 */
function vocabulary_admin_summary($task, $subtask) {
  $task_name = page_manager_make_task_name($task['name'], $subtask['name']);

  $rows[] = array(
    array('class' => t('page-summary-label'), 'data' => t('Path')),
    array('class' => t('page-summary-data'), 'data' => 'taxonomy/%taxonomy_vocabulary'),
    array('class' => t('page-summary-operation'), 'data' => ''),
  );

  $rows[] = array(
    array('class' => t('page-summary-label'), 'data' => t('Access')),
    array('class' => t('page-summary-data'), 'data' => t('This page is publicly accessible.')),
    array('class' => t('page-summary-operation'), 'data' => ''),
  );

  $menu = t('No menu entry');

  $rows[] = array(
    array('class' => t('page-summary-label'), 'data' => t('Menu')),
    array('class' => t('page-summary-data'), 'data' => $menu),
    array('class' => t('page-summary-operation'), 'data' => ''),
  );


  $rows[] = array(
    array('class' => t('page-summary-label'), 'data' => t('%taxonomy_vocabulary')),
    array('class' => t('page-summary-data'), 'data' => ''),
    array('class' => t('page-summary-operation'), 'data' => ''),
  );

  $output = theme('table', array(), $rows, array('id' => 'page-manager-page-summary'));
  return $output;
}


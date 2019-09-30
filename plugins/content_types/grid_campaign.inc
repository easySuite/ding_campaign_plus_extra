<?php

/**
 * @file
 * Ding Campaign CTools plugin file.
 */

$plugin = array(
  'title' => t('Grid of campaigns'),
  'description' => t('Display grid of campaigns'),
  'single' => TRUE,
  'defaults' => array(
    'ding_campaign_count' => 6,
    'ding_campaign_offset' => 0,
    'single' => TRUE,
  ),
  'edit form' => 'ding_campaign_plus_extra_grid_campaign_content_type_edit_form',
  'render callback' => 'ding_campaign_plus_extra_grid_campaign_content_type_render',
  'required context' => array(
    new ctools_context_optional(t('Page node'), 'node'),
    new ctools_context_optional(t('Related library'), 'node'),
    new ctools_context_optional(t('Search term'), 'string'),
    new ctools_context_optional(t('Taxonomy term'), array('entity:taxonomy_term', 'taxonomy_term')),
  ),
  'category' => t('Ding!'),
);

/**
 * Implements hook_content_type_render().
 */
function ding_campaign_plus_extra_grid_campaign_content_type_render($subtype, $conf, $panel_args, $context) {
  $block = new stdClass();
  $currentResult = ting_search_current_results();
  $context_data = array(
    'page' => isset($context[0]->data) ? $context[0]->data : NULL,
    'library' => isset($context[1]->data) ? $context[1]->data : NULL,
    'search_term' => isset($context[2]->data) ? $context[2]->data : NULL,
    'taxonomy_term' => isset($context[3]->data) ? $context[3]->data : NULL,
    'path' => (object) array(
      'raw' => arg(),
      'path' => drupal_get_path_alias($_GET['q']),
    ),
    'search_request' => $currentResult ? $currentResult->getSearchRequest() : NULL,
  );
  $style = !empty($conf['style']) ? $conf['style'] : 'ribbon';
  $count = $conf['ding_campaign_count'];
  $offset = $conf['ding_campaign_offset'];
  $more_link = $conf['ding_campaign_more_link'];
  // Set default value if it was not set in panel settings.
  $width = (!empty($conf['ding_campaign_width'])) ? $conf['ding_campaign_width'] : 'ding_campaign_full_width';
  $widget = 'grid_campaign';

  $cid = sha1(serialize([$widget, $context_data]));
  $cache = cache_get($cid);
  $content = '';
  if (isset($cache->data) && REQUEST_TIME < $cache->expire) {
    $content = $cache->data;
  }
  else {
    //TODO: Create a single widget with multiple display modes.
    cache_set($cid . '-extra', array($context_data, $widget, $style, $count, $offset, $more_link, $width));
    $content = '<div data-ding-campaign-plus-extra-cid="' . $cid . '" class="ding-campaing-plus-extra-placeholder"></div>';
    drupal_add_js(drupal_get_path('module', 'ding_campaign_plus_extra') . '/js/ding_campaign_plus_extra.js');
  }

  $block->content = $content;

  return $block;
}

/**
 * Implements hook_content_type_edit_form().
 */
function ding_campaign_plus_extra_grid_campaign_content_type_edit_form($form, &$form_state) {
  $form['style'] = array(
    '#type' => 'select',
    '#title' => t('Campaign style'),
    '#description' => t('The display type of the campaigns in this pane.'),
    '#options' => array(
      'ribbon' => t('Ribbon'),
      'box' => t('Box'),
    ),
    '#default_value' => $form_state['conf']['style'],
  );
  $form['ding_campaign_count'] = array(
    '#type' => 'textfield',
    '#title' => t('Campaign count'),
    '#description' => t('The maximum number of campaigns to display'),
    '#default_value' => (isset($form_state['conf']['ding_campaign_count'])) ? $form_state['conf']['ding_campaign_count'] : 3,
    '#size' => 2,
    '#maxlength' => 2,
  );

  $form['ding_campaign_offset'] = array(
    '#type' => 'textfield',
    '#title' => t('Campaign offset'),
    '#description' => t('Which campaign should we count from when returning campaigns. This is used to avoid displaying the same campaigns when using multiple campaign panes on the same panel. Use 0 to return the most relevant campaigns.'),
    '#default_value' => (isset($form_state['conf']['ding_campaign_offset'])) ? $form_state['conf']['ding_campaign_offset'] : 0,
    '#size' => 3,
    '#maxlength' => 3,
  );

  $form['ding_campaign_width'] = array(
    '#type' => 'select',
    '#title' => t('Campaign width'),
    '#description' => t('Applicable for campaigns. Content will be resized to selected width.'),
    '#options' => array(
      'ding_campaign_medium_width' => t('Medium width'),
      'ding_campaign_full_width' => t('Full width'),
    ),
    '#default_value' => (isset($form_state['conf']['ding_campaign_width'])) ? $form_state['conf']['ding_campaign_width'] : 'ding_campaign_full_width',
  );

  $form['ding_campaign_more_link'] = array(
    '#type' => 'fieldset',
    '#title' => t('More link'),
    '#description' => t('Allows to add "More link" button into the bottom of grid campaign'),
    '#collapsed' => FALSE,
  );

  $form['ding_campaign_more_link']['link_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Link title'),
    '#default_value' => (isset($form_state['conf']['ding_campaign_more_link']['link_title'])) ? $form_state['conf']['ding_campaign_more_link']['link_title'] : '',
  );

  $form['ding_campaign_more_link']['link_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Link URL'),
    '#default_value' => (isset($form_state['conf']['ding_campaign_more_link']['link_url'])) ? $form_state['conf']['ding_campaign_more_link']['link_url'] : '',
  );

  return $form;
}

/**
 * Implements hook_content_type_edit_form_validate().
 */
function ding_campaign_plus_extra_grid_campaign_content_type_edit_form_validate($form, &$form_state) {
  // Sanitise the submitted values.
  $values = array(
    'ding_campaign_count' => 1,
    'ding_campaign_offset' => 0,
  );
  foreach ($values as $value => $min_value) {
    $val = trim($form_state['values'][$value]);
    $count = intval($val);

    // Give an error if a value less than 1 was entered.
    if (!empty($val) && $count < $min_value) {
      form_set_error($value, t('%value must be a number larger than zero.',
          array(
            '%value' => drupal_ucfirst(str_replace('_', ' ', $value)),
          )));
    }
    // Otherwise, store the sanitised value in the form state.
    else {
      $form_state['values'][$value] = $count;
    }
  }
}

/**
 * Implements hook_content_type_edit_form_submit().
 */
function ding_campaign_plus_extra_grid_campaign_content_type_edit_form_submit($form, &$form_state) {
  $form_state['conf']['style'] = $form_state['values']['style'];
  $form_state['conf']['ding_campaign_count'] = $form_state['values']['ding_campaign_count'];
  $form_state['conf']['ding_campaign_offset'] = $form_state['values']['ding_campaign_offset'];
  $form_state['conf']['ding_campaign_width'] = $form_state['values']['ding_campaign_width'];
  $form_state['conf']['ding_campaign_more_link'] = array(
    'link_title' => $form_state['values']['link_title'],
    'link_url' => $form_state['values']['link_url'],
  );
}

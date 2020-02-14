<?php
/**
 * @file
 * Ding single campaign CTools plugins file.
 */

$plugin = array(
  'title' => t('Single campaigns'),
  'description' => t('Display single campaigns'),
  'single' => TRUE,
  'edit form' => 'ding_campaign_single_campaign_content_type_edit_form',
  'render callback' => 'ding_campaign_single_campaign_content_type_render',
  'category' => t('Ding!'),
);

/**
 * Implements hook_content_type_edit_form().
 */
function ding_campaign_single_campaign_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];
  $available_campaigns = ding_campaign_plus_extra_get_list();
  $form['ding_campaign']['single_campaign'] = array(
    '#type' => 'select',
    '#title' => t('Select campaign'),
    '#options' => $available_campaigns,
    '#default_value' => (!empty($conf['single_campaign'])) ? $conf['single_campaign'] : reset($available_campaigns),
  );

  return $form;
}

/**
 * Fetch available campaigns.
 *
 * @return array
 *   Array of campaigns with ids and titles.
 */
function ding_campaign_plus_extra_get_list() {
  $nids = db_select('node', 'n')
    ->fields('n', ['nid', 'title'])
    ->condition('type', 'ding_campaign_plus')
    ->condition('status', 1)
    ->execute()
    ->fetchAllAssoc('nid');

  $campaigns = array();

  foreach ($nids as $k => $v) {
    $campaigns[$v->nid] = $v->title;
  }

  return $campaigns;
}
/**
 * Submit function for configuration form.
 *
 * @see ding_campaign_single_campaign_content_type_edit_form().
 */
function ding_campaign_single_campaign_content_type_edit_form_submit($form, &$form_state) {
  $form_state['conf']['single_campaign'] = $form_state['values']['single_campaign'];
}

/**
 * Implements hook_content_type_render().
 */
function ding_campaign_single_campaign_content_type_render($subtype, $conf, $panel_args, $context) {
  $campaign = entity_load('node', [$conf['single_campaign']]);
  if (empty($campaign)) {
    return '';
  }

  $block = new stdClass();
  $block->content = [
    '#theme' => 'ding_campaign_wrapper',
    '#content' => node_view(reset($campaign), 'full'),
  ];

  return $block;
}

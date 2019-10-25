<?php
/**
 * @file
 * ding_campaign_plus_extra.features.inc
 */

/**
 * Implements hook_image_default_styles().
 */
function ding_campaign_plus_extra_image_default_styles() {
  $styles = array();

  // Exported image style: ding_campaign_grid.
  $styles['ding_campaign_grid'] = array(
    'name' => 'ding_campaign_grid',
    'label' => 'Ding campaign grid',
    'effects' => array(
      1 => array(
        'label' => 'Scale and crop',
        'help' => 'Scale and crop will maintain the aspect-ratio of the original image, then crop the larger dimension. This is most useful for creating perfectly square thumbnails without stretching the image.',
        'effect callback' => 'image_scale_and_crop_effect',
        'dimensions callback' => 'image_resize_dimensions',
        'form callback' => 'image_resize_form',
        'summary theme' => 'image_resize_summary',
        'module' => 'image',
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => 355,
          'height' => 200,
        ),
        'weight' => 1,
      ),
    ),
  );

  return $styles;
}

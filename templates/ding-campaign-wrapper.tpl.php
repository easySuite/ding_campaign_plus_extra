<?php
/**
 * @file
 * Default template for image campaigns.
 *
 * @var string|null $campaign_link
 * @var string|null $campaign_image
 * @var string $title
 */
$image = "<img src='$campaign_image' alt='$title' />";
?>
<div class="ding-campaign-wrapper">
  <?php print l($image, $campaign_link, ['html' => TRUE, 'attributes' => ['title' => $title]]); ?>
</div>

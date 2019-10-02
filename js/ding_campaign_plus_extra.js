/**
 * @file
 * Handle campaigns.
 */

(function ($) {
  'use strict';

  /**
   * Load lazy loaded campaigns.
   */
  Drupal.behaviors.ding_campaign_plus_extra_lazy_load = {
    attach: function (context) {
      $('[data-ding-campaign-plus-extra-cid]', context).once(function () {
        var campaign = $(this);
        var cid = campaign.data('ding-campaign-plus-extra-cid');
        var url = Drupal.settings.basePath + Drupal.settings.pathPrefix + 'ding_campaigns_plus_extra/' + cid;
        $.get(url, {}, function (response) {
          campaign.replaceWith(response);
        });

      });
    }
  };
})(jQuery);

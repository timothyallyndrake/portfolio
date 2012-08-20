<?php

function theme_commerce_fedex_tracking_title($variables) {
  return array(
    '#type' => 'item',
    '#title' => $variables['tracking_number'],
  );
}

function theme_commerce_fedex_tracking_table($variables) {
  return array(
    '#theme' => 'table',
    '#empty' => t('No tracking information available.'),
    '#header' => array(
      t('Date/Time'),
      t('Activity'),
      t('Location'),
      t('Details'),
    ),
    '#rows' => theme('commerce_fedex_tracking_table_rows', array('events' => $variables['tracking']->Events)),
  );
}

function theme_commerce_fedex_tracking_table_rows($variables) {
  $render = array();

  if (empty($variables['events']) || !is_array($variables['events'])) {
    return $render;
  }

  foreach ($variables['events'] as $event) {
    $render[] = array(    
      format_date(strtotime($event->Timestamp), 'custom', 'M j, o g:i A'),
      t($event->EventDescription),
      ((empty($event->Address->City)) ? 'Unavailable' : $event->Address->City),
      $event->StatusExceptionDescription
    );
  }

  return $render;
}

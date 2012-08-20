<?php

/**
 * Implements hook_entity_property_info().
 */
function commerce_fedex_entity_property_info() {
  $property_info = array();

  $properties = &$property_info['commerce_fedex_shipment']['properties'];

  $properties['shipment_id'] = array(
    'label' => t('Shipment ID'),
    'description' => t('Commerce FedEx shipment ID'),
    'type' => 'integer',
    'schema field' => 'shipment_id',
  );

  $properties['delivery_id'] = array(
    'label' => t('Delivery ID'),
    'description' => t('Commerce delivery ID'),
    'type' => 'integer',
    'required' => TRUE,
    'schema field' => 'delivery_id',
    'getter callback' => 'entity_property_verbatim_get',
    'setter callback' => 'entity_property_verbatim_set',
  );

  $properties['commerce_delivery'] = $properties['delivery_id'];
  $properties['commerce_delivery']['type'] = 'commerce_delivery';
  $properties['commerce_delivery']['getter callback'] = 'commerce_fedex_shipment_get_properties';

  $properties['label_fid'] = array(
    'label' => t('Label file ID'),
    'description' => t('Commerce FedEx shipment label file ID'),
    'type' => 'integer',
    'required' => TRUE,
    'schema field' => 'label_fid',
    'getter callback' => 'entity_property_verbatim_get',
    'setter callback' => 'entity_property_verbatim_set',
  );

  $properties['label_uri'] = array(
    'label' => t('Label URI'),
    'description' => t('Commerce FedEx shipment label URI'),
    'type' => 'uri',
    'schema field' => 'tracking_number',
    'getter callback' => 'commerce_fedex_shipment_get_properties',
  );

  $properties['dropoff_type'] = array(
    'label' => t('Dropoff type'),
    'description' => t('Commerce FedEx shipment dropoff type'),
    'type' => 'text',
    'required' => TRUE,
    'schema field' => 'dropoff_type',
    'getter callback' => 'entity_property_verbatim_get',
    'setter callback' => 'entity_property_verbatim_set',
  );

  $properties['service_type'] = array(
    'label' => t('Service type'),
    'description' => t('Commerce FedEx shipment service type'),
    'type' => 'text',
    'required' => TRUE,
    'schema field' => 'service_type',
    'getter callback' => 'entity_property_verbatim_get',
    'setter callback' => 'entity_property_verbatim_set',
  );

  $properties['tracking_type'] = array(
    'label' => t('Tracking type'),
    'description' => t('Commerce FedEx shipment tracking type'),
    'type' => 'text',
    'required' => TRUE,
    'schema field' => 'tracking_type',
    'getter callback' => 'entity_property_verbatim_get',
    'setter callback' => 'entity_property_verbatim_set',
  );

  $properties['tracking_number'] = array(
    'label' => t('Tracking #'),
    'description' => t('Commerce FedEx shipment tracking number'),
    'type' => 'text',
    'required' => TRUE,
    'schema field' => 'tracking_number',
    'getter callback' => 'entity_property_verbatim_get',
    'setter callback' => 'entity_property_verbatim_set',
  );

  $properties['tracking_cache'] = array(
    'label' => t('Tracking cache'),
    'description' => t('Commerce FedEx shipment tracking cache'),
    'type' => 'text',
    'required' => FALSE,
    'schema field' => 'tracking_cache',
    'getter callback' => 'entity_property_verbatim_get',
    'setter callback' => 'entity_property_verbatim_set',
  );

  $properties['is_new'] = array(
    'label' => t('Is new'),
    'description' => t('Commerce Fedex shipment is new'),
    'type' => 'boolean',
    'schema field' => 'shipment_id',
    'getter callback' => 'commerce_fedex_shipment_get_properties',
  );

  return $property_info;
}

/**
 * Implements hook_entity_property_info_alter().
 */
function commerce_fedex_entity_property_info_alter(&$property_info) {
  $property_info['commerce_delivery']['properties']['commerce_fedex_shipment'] = array(
    'label' => t('Shipment entity'),
    'description' => t('Commerce FedEx shipment entity'),
    'type' => 'commerce_fedex_shipment',
    'getter callback' => 'commerce_fedex_shipment_get_properties',
  );
}

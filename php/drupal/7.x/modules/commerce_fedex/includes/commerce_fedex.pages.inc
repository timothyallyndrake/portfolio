<?php

function commerce_fedex_settings_form($form, &$form_state) {
  $form['identification'] = array(
    '#type' => 'fieldset',
    '#title' => t('Identification'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );

  $form['identification']['commerce_fedex_account_number'] = array(
    '#type' => 'textfield',
    '#title' => 'Account #',
    '#default_value' => variable_get('commerce_fedex_account_number'),
    '#required' => TRUE,
    '#size' => 50,
  );

  $form['identification']['commerce_fedex_meter_number'] = array(
    '#type' => 'textfield',
    '#title' => 'Meter #',
    '#default_value' => variable_get('commerce_fedex_meter_number'),
    '#required' => TRUE,
    '#size' => 50,
  );

  $form['authentication'] = array(
    '#type' => 'fieldset',
    '#title' => t('Authentication'),
  );

  $form['authentication']['commerce_fedex_developer_key'] = array(
    '#type' => 'textfield',
    '#title' => 'Developer Key',
    '#default_value' => variable_get('commerce_fedex_developer_key'),
    '#required' => TRUE,
    '#size' => 50,
  );

  $form['authentication']['commerce_fedex_developer_password'] = array(
    '#type' => 'textfield',
    '#title' => 'Developer Password',
    '#default_value' => variable_get('commerce_fedex_developer_password'),
    '#required' => TRUE,
    '#size' => 50,
  );

  $form['shipper'] = array(
    '#type' => 'fieldset',
    '#title' => t('Shipper'),
  );

  $form['shipper']['commerce_fedex_shipper_company_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Company Name'),
    '#default_value' => variable_get('commerce_fedex_shipper_company_name'),
    '#required' => TRUE,
    '#size' => 50,
  );

  $form['shipper']['commerce_fedex_shipper_person_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Contact'),
    '#default_value' => variable_get('commerce_fedex_shipper_person_name'),
    '#required' => TRUE,
    '#size' => 50,
  );

  $form['shipper']['commerce_fedex_shipper_phone_number'] = array(
    '#type' => 'textfield',
    '#title' => t('Phone Number'),
    '#default_value' => variable_get('commerce_fedex_shipper_phone_number'),
    '#required' => TRUE,
    '#size' => 50,
  );

  $form['shipper']['commerce_fedex_shipper_street_line_1'] = array(
    '#type' => 'textfield',
    '#title' => t('Street (line 1)'),
    '#default_value' => variable_get('commerce_fedex_shipper_street_line_1'),
    '#required' => TRUE,
    '#size' => 100,
  );

  $form['shipper']['commerce_fedex_shipper_street_line_2'] = array(
    '#type' => 'textfield',
    '#title' => t('Street (line 2)'),
    '#default_value' => variable_get('commerce_fedex_shipper_street_line_2'),
    '#size' => 100,
  );

  $form['shipper']['commerce_fedex_shipper_city'] = array(
    '#type' => 'textfield',
    '#title' => t('City'),
    '#default_value' => variable_get('commerce_fedex_shipper_city'),
    '#required' => TRUE,
    '#size' => 50,
  );

  $form['shipper']['commerce_fedex_shipper_state_or_province_code'] = array(
    '#type' => 'textfield',
    '#title' => t('State or province code'),
    '#default_value' => variable_get('commerce_fedex_shipper_state_or_province_code'),
    '#required' => TRUE,
    '#size' => 5,
  );

  $form['shipper']['commerce_fedex_shipper_postal_code'] = array(
    '#type' => 'textfield',
    '#title' => t('Postal code'),
    '#default_value' => variable_get('commerce_fedex_shipper_postal_code'),
    '#required' => TRUE,
    '#size' => 5,
  );

  $form['shipper']['commerce_fedex_shipper_country_code'] = array(
    '#type' => 'select',
    '#title' => t('Country'),
    '#default_value' => variable_get('commerce_fedex_shipper_country_code'),
    '#default_value' => 'US',
    '#options' => _addressfield_country_options_list(),
  );

  return system_settings_form($form);
}

function commerce_fedex_shipment_form($form, &$form_state, $delivery) {
  $delivery = entity_metadata_wrapper('commerce_delivery', $delivery);
  $form_state['delivery'] = $delivery;

  $order = entity_metadata_wrapper('commerce_order', $delivery->order_id->value());

  $shipping = $order->commerce_customer_shipping;
  $form_state['shipping'] = $shipping;

  $shipping_address = $shipping->commerce_customer_address;
  $form_state['shipping_address'] = $shipping_address;

  if (!($shipment = $delivery->commerce_fedex_shipment->value())) {
    $shipment = commerce_fedex_shipment_create(array('delivery_id' => $delivery->delivery_id->value()));
    $shipment->delivery_id = $delivery->delivery_id->value();
  }

  $shipment = entity_metadata_wrapper('commerce_fedex_shipment', $shipment);
  $form_state['shipment'] = $shipment;

  if (!$shipment->is_new->value()) {
    $form['tracking'] = array(
      '#type' => 'fieldset',
      '#title' => t('Tracking'),
    );

    $form['tracking']['type'] = array(
      '#type' => 'item',
      '#title' => t('Shipping Method'),
      '#markup' => $shipment->tracking_type->value()
    );

    $form['tracking']['number'] = array(
      '#type' => 'item',
      '#title' => t('Tracking #'),
      '#markup' => $shipment->tracking_number->value()
    );

    $form['tracking']['label'] = array(
      '#type' => 'item',
      '#title' => t('Shipping Label'),
      '#markup' => l(t('View PDF'), file_create_url($shipment->label_uri->value()), array('attributes' => array('target' => '_blank'))),
    );
  }

  $form['shipment'] = array(
    '#type' => 'fieldset',
    '#title' => t('Shipment'),
  );

  $form['shipment']['shipment_dropoff_type'] = array(
    '#type' => 'select',
    '#title' => t('Dropoff type'),
    '#description' => t('Method by which the package is to be tendered to FedEx.'),
    '#default_value' => $shipment->dropoff_type->value(),
    '#options' => CommerceFedExDropoffType::getAllAssoc(),
  );

  $form['shipment']['shipment_service_type'] = array(
    '#type' => 'select',
    '#title' => t('Service type'),
    '#description' => t('Available FedEx service options. (Residential recipients require ground home delivery.)'),
    '#default_value' => $shipment->service_type->value(),
    '#options' => CommerceFedExServiceType::getAllAssoc(),
  );

  $form['shipper'] = array(
    '#type' => 'fieldset',
    '#title' => t('Shipper'),
  );

  $form['shipper']['shipper_company_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Company Name'),
    '#default_value' => variable_get('commerce_fedex_shipper_company_name'),
    '#required' => TRUE,
    '#size' => 50,
  );

  $form['shipper']['shipper_person_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Contact'),
    '#default_value' => variable_get('commerce_fedex_shipper_person_name'),
    '#required' => TRUE,
    '#size' => 50,
  );

  $form['shipper']['shipper_phone_number'] = array(
    '#type' => 'textfield',
    '#title' => t('Phone Number'),
    '#default_value' => variable_get('commerce_fedex_shipper_phone_number'),
    '#required' => TRUE,
    '#size' => 50,
  );

  $form['shipper']['shipper_street_line_1'] = array(
    '#type' => 'textfield',
    '#title' => t('Street (line 1)'),
    '#default_value' => variable_get('commerce_fedex_shipper_street_line_1'),
    '#required' => TRUE,
    '#size' => 100,
  );

  $form['shipper']['shipper_street_line_2'] = array(
    '#type' => 'textfield',
    '#title' => t('Street (line 2)'),
    '#default_value' => variable_get('commerce_fedex_shipper_street_line_2'),
    '#size' => 100,
  );

  $form['shipper']['shipper_city'] = array(
    '#type' => 'textfield',
    '#title' => t('City'),
    '#default_value' => variable_get('commerce_fedex_shipper_city'),
    '#required' => TRUE,
    '#size' => 50,
  );

  $form['shipper']['shipper_state_or_province_code'] = array(
    '#type' => 'textfield',
    '#title' => t('State or province code'),
    '#default_value' => variable_get('commerce_fedex_shipper_state_or_province_code'),
    '#required' => TRUE,
    '#size' => 5,
  );

  $form['shipper']['shipper_postal_code'] = array(
    '#type' => 'textfield',
    '#title' => t('Postal code'),
    '#default_value' => variable_get('commerce_fedex_shipper_postal_code'),
    '#required' => TRUE,
    '#size' => 5,
  );

  $form['shipper']['shipper_country_code'] = array(
    '#type' => 'select',
    '#title' => t('Country'),
    '#default_value' => variable_get('commerce_fedex_shipper_country_code'),
    '#default_value' => 'US',
    '#options' => _addressfield_country_options_list(),
  );

  $form['recipient'] = array(
    '#type' => 'fieldset',
    '#title' => t('recipient'),
  );

  $form['recipient']['recipient_person_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Contact'),
    '#default_value' => $shipping_address->name_line->value(),
    '#required' => TRUE,
    '#size' => 50,
  );

  $form['recipient']['recipient_phone_number'] = array(
    '#type' => 'textfield',
    '#title' => t('Phone Number'),
    '#default_value' => $shipping->field_phone_number->value(),
    '#required' => TRUE,
    '#size' => 50,
  );

  $form['recipient']['recipient_street_line_1'] = array(
    '#type' => 'textfield',
    '#title' => t('Street (line 1)'),
    '#default_value' => $shipping_address->thoroughfare->value(),
    '#required' => TRUE,
    '#size' => 100,
  );

  $form['recipient']['recipient_street_line_2'] = array(
    '#type' => 'textfield',
    '#title' => t('Street (line 2)'),
    '#default_value' => $shipping_address->premise->value(),
    '#size' => 100,
  );

  $form['recipient']['recipient_city'] = array(
    '#type' => 'textfield',
    '#title' => t('City'),
    '#default_value' => $shipping_address->locality->value(),
    '#required' => TRUE,
    '#size' => 50,
  );

  $form['recipient']['recipient_state_or_province_code'] = array(
    '#type' => 'textfield',
    '#title' => t('State or province code'),
    '#default_value' => $shipping_address->administrative_area->value(),
    '#required' => TRUE,
    '#size' => 5,
  );

  $form['recipient']['recipient_postal_code'] = array(
    '#type' => 'textfield',
    '#title' => t('Postal code'),
    '#default_value' => $shipping_address->postal_code->value(),
    '#required' => TRUE,
    '#size' => 5,
  );

  $form['recipient']['recipient_country_code'] = array(
    '#type' => 'select',
    '#title' => t('Country'),
    '#default_value' => $shipping_address->country->value(),
    '#options' => _addressfield_country_options_list(),
  );

  $note = $shipment->is_new->value()
    ? 'Create shipment - Creates a new shipment request with the FedEx system, attaches the newly ' .
      'generated shipping label to this delivery and exposes tracking information to the purchaser.'
    : 'Update shipment - Cancels the existing shipment request with the FedEx system before creating ' .
      'a new one, detaches and deletes the existing shipping label from this delivery before regenerating ' .
      'and reattaching the new one to this delivery, and exposing the new tracking information to the purchaser.';

  $form['note'] = array(
    '#type' => 'item',
    '#description' => t('Note: %note', array('%note' => $note)),
  );

  $form['actions'] = array('#type' => 'actions');

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#submit' => array('commerce_fedex_shipment_form_submit'),
    '#value' => t('@action shipment', array('@action' => $shipment->is_new->value() ? 'Create' : 'Update')),
  );

  if (!$shipment->is_new->value()) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#submit' => array('commerce_fedex_shipment_delete_submit'),
      '#value' => t('Delete shipment'),
    );
  }

  return $form;
}

function commerce_fedex_shipment_delete_submit($form, &$form_state) {
  $delivery = $form_state['delivery'];

  $form_state['redirect'] = 'admin/commerce/deliveries/' . $delivery->delivery_id->value() . '/shipment/delete';
}

function commerce_fedex_shipment_delete_confirm($form, &$form_state, $delivery) {
  $delivery = entity_metadata_wrapper('commerce_delivery', $delivery);
  $form_state['delivery'] = $delivery;

  $question = t('Are you sure you want to delete this shipment?');
  $path = 'admin/commerce/deliveries/' . $delivery->delivery_id->value() . '/shipment';
  $description = t('This action cannot be undone.');
  $yes = t('Confirm');
  $no = t('Cancel');

  return confirm_form($form, $question, $path, $description, $yes, $no);
}

function commerce_fedex_shipment_delete_confirm_submit($form, &$form_state) {
  $delivery = $form_state['delivery'];
  $shipment = $delivery->commerce_fedex_shipment;

  commerce_fedex_shipment_shipping_label_file_delete($shipment);
  $shipment->delete();

  $form_state['redirect'] = 'admin/commerce/deliveries/' . $delivery->delivery_id->value() . '/shipment';
}

function commerce_fedex_shipment_form_validate($form, &$form_state) {
  $values = (object) $form_state['values'];

  $shipment = $form_state['shipment'];

  $request = new CommerceFedExShipment();

  if (!$shipment->is_new->value()) {
    commerce_fedex_shipment_shipping_label_file_delete($shipment);

    $delete_input = new CommerceFedExShipmentDeleteInput();
    $delete_input->TrackingId->TrackingIdType = $shipment->tracking_type;
    $delete_input->TrackingId->TrackingNumber = $shipment->tracking_number;

    $request->delete($delete_input);
  }

  $create_input = new CommerceFedExShipmentCreateInput();
  $create_input->DropoffType = $values->shipment_dropoff_type;
  $create_input->ServiceType = $values->shipment_service_type;
  $create_input->Shipper->Contact->CompanyName = $values->shipper_company_name;
  $create_input->Shipper->Contact->PersonName = $values->shipper_person_name;
  $create_input->Shipper->Contact->PhoneNumber = $values->shipper_phone_number;
  $create_input->Shipper->Address->StreetLine1 = $values->shipper_street_line_1;
  $create_input->Shipper->Address->StreetLine2 = $values->shipper_street_line_2;
  $create_input->Shipper->Address->City = $values->shipper_city;
  $create_input->Shipper->Address->StateOrProvinceCode = $values->shipper_state_or_province_code;
  $create_input->Shipper->Address->PostalCode = $values->shipper_postal_code;
  $create_input->Shipper->Address->CountryCode = $values->shipper_country_code;
  $create_input->Recipient->Contact->PersonName = $values->recipient_person_name;
  $create_input->Recipient->Contact->PhoneNumber = $values->recipient_phone_number;
  $create_input->Recipient->Address->StreetLine1 = $values->recipient_street_line_1;
  $create_input->Recipient->Address->StreetLine2 = $values->recipient_street_line_2;
  $create_input->Recipient->Address->City = $values->recipient_city;
  $create_input->Recipient->Address->StateOrProvinceCode = $values->recipient_state_or_province_code;
  $create_input->Recipient->Address->PostalCode = $values->recipient_postal_code;
  $create_input->Recipient->Address->CountryCode = $values->recipient_country_code;

  if (!($response = $request->create($create_input))) {
    return FALSE;
  }

  $form_state['create_input'] = $create_input;
  $form_state['response'] = $response;

  return TRUE;
}

function commerce_fedex_shipment_form_submit($form, &$form_state) {
  $create_input = $form_state['create_input'];

  $shipment = $form_state['shipment'];

  $shipment->dropoff_type = $create_input->DropoffType;
  $shipment->service_type = $create_input->ServiceType;

  variable_set('commerce_fedex_shipper_company_name', $create_input->Shipper->Contact->CompanyName);
  variable_set('commerce_fedex_shipper_shipper_person_name', $create_input->Shipper->Contact->PersonName);
  variable_set('commerce_fedex_shipper_shipper_phone_number', $create_input->Shipper->Contact->PhoneNumber);
  variable_set('commerce_fedex_shipper_shipper_street_line_1', $create_input->Shipper->Address->StreetLine1);
  variable_set('commerce_fedex_shipper_shipper_street_line_2', $create_input->Shipper->Address->StreetLine2);
  variable_set('commerce_fedex_shipper_shipper_city', $create_input->Shipper->Address->City);
  variable_set('commerce_fedex_shipper_state_or_province_code', $create_input->Shipper->Address->StateOrProvinceCode);
  variable_set('commerce_fedex_shipper_postal_code', $create_input->Shipper->Address->PostalCode);
  variable_set('commerce_fedex_shipper_country_code', $create_input->Shipper->Address->CountryCode);

  $shipping = $form_state['shipping'];
  $shipping_address = $form_state['shipping_address'];

  $shipping_address->name_line = $create_input->Recipient->Contact->PersonName;
  $shipping->field_phone_number = $create_input->Recipient->Contact->PhoneNumber;
  $shipping_address->thoroughfare = $create_input->Recipient->Address->StreetLine1;
  $shipping_address->premise = $create_input->Recipient->Address->StreetLine2;
  $shipping_address->locality = $create_input->Recipient->Address->City;
  $shipping_address->administrative_area = $create_input->Recipient->Address->StateOrProvinceCode;
  $shipping_address->postal_code = $create_input->Recipient->Address->PostalCode;
  $shipping_address->country = $create_input->Recipient->Address->CountryCode;

  $response = $form_state['response'];

  $shipment->tracking_type = $response->tracking_type;
  $shipment->tracking_number = $response->tracking_number;

  commerce_fedex_shipment_shipping_label_file_create($shipment, $response);

  $shipping->save();
  $shipment->save();

  $form_state['redirect'] = drupal_get_destination();
}

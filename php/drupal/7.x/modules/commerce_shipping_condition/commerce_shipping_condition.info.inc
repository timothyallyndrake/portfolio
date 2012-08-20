<?php

class CommerceShippingConditionTypeMetadataController extends EntityDefaultMetadataController {

  public function entityPropertyInfo() {
    $info = parent::entityPropertyInfo();

    $properties = &$info[$this->type]['properties'];

    $properties['id'] = array(
      'label' => t('ID'),
      'description' => t('Unique identifier of this shipping condition type.'),
      'type' => 'integer',
      'schema field' => 'id',
    );

    $properties['type'] = array(
      'label' => t("Type"),
      'description' => t('Unique, machine-readable identifier of this shipping condition type.'),
      'type' => 'token',
      'required' => TRUE,
      'schema field' => 'type',
      'setter callback' => 'entity_property_verbatim_set',
    );

    $properties['name'] = array(
      'label' => t('Name'),
      'description' => t('Non-unique, human-readable identifier of this shipping condition type.'),
      'type' => 'text',
      'required' => TRUE,
      'schema field' => 'name',
    );

    $properties['description'] = array(
      'label' => t('Description'),
      'description' => t('Description of this shipping condition type.'),
      'type' => 'text',
      'required' => TRUE,
      'schema field' => 'description',
    );

    $properties['status'] = array(
      'label' => t('Status'),
      'description' => t('Active/inactive status of this shipping condition type.'),
      'type' => 'boolean',
      'required' => TRUE,
      'schema field' => 'status',
      'setter callback' => 'entity_property_verbatim_set',
    );

    return $info;
  }
}

class CommerceShippingConditionMetadataController extends EntityDefaultMetadataController {

  /**
   * Implements hook_entity_property_info().
   */
  public function entityPropertyInfo() {
    $info = parent::entityPropertyInfo();

    $properties = &$info[$this->type]['properties'];

    $properties['id'] = array(
      'label' => t('ID'),
      'description' => t('Unique identifier of this shipping condition.'),
      'type' => 'integer',
      'schema field' => 'id',
    );

    $properties['type'] = array(
      'label' => t("Type"),
      'description' => t('Unique, machine-readable identifier of this shipping condition.'),
      'type' => 'commerce_shipping_condition_type',
      'required' => TRUE,
      'schema field' => 'type',
      'getter callback' => 'entity_property_getter_method',
      'setter callback' => 'entity_property_verbatim_set',
      'options list' => 'commerce_shipping_condition_type_options_list',
    );

    $properties['name'] = array(
      'label' => t('Name'),
      'description' => t('Non-unique, human-readable identifier of this shipping condition.'),
      'type' => 'text',
      'required' => FALSE,
      'schema field' => 'name',
    );

    $properties['description'] = array(
      'label' => t('Description'),
      'description' => t('Description of this shipping condition.'),
      'type' => 'text',
      'required' => FALSE,
      'schema field' => 'description',
    );

    $properties['status'] = array(
      'label' => t('Status'),
      'description' => t('Active/inactive status of this shipping condition.'),
      'type' => 'boolean',
      'required' => TRUE,
      'schema field' => 'status',
      'setter callback' => 'entity_property_verbatim_set',
    );

    $properties['commerce_flat_rate_name'] = array(
      'label' => t("Shipping Service"),
      'description' => t('Commerce shipping service (flat rate) of this shipping condition.'),
      'type' => 'text',
      'required' => TRUE,
      'schema field' => 'commerce_flat_rate_name',
      'getter callback' => 'entity_property_verbatim_get',
      'setter callback' => 'entity_property_verbatim_set',
      'options list' => 'commerce_shipping_condition_commerce_shipping_service_option_list',
    );

    $properties['commerce_product_type'] = array(
      'label' => t("Product Type"),
      'description' => t('Commerce product type of this shipping condition.'),
      'type' => 'text',
      'required' => FALSE,
      'schema field' => 'commerce_product_type',
      'getter callback' => 'entity_property_verbatim_get',
      'setter callback' => 'entity_property_verbatim_set',
      'options list' => 'commerce_product_type_options_list',
    );

    $properties['country_code'] = array(
      'label' => t('Country'),
      'description' => t('Two letter ISO country code of this shipping condition.'),
      'type' => 'text',
      'required' => FALSE,
      'schema field' => 'country_code',
      'getter callback' => 'entity_property_verbatim_get',
      'setter callback' => 'entity_property_verbatim_set',
      'options list' => '_addressfield_country_options_list',
    );

    $properties['rate'] = array(
      'label' => t('Rate'),
      'description' => t('Monetary rate of this shipping condition.'),
      'type' => 'decimal',
      'required' => TRUE,
      'schema field' => 'rate',
    );

    return $info;
  }
}

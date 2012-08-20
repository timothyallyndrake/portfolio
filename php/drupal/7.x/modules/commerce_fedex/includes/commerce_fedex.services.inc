<?php

/**
 * CommerceFedExType
 */
class CommerceFedExType {
  protected static function getAllAssoc($class) {
    $reflection = new ReflectionClass($class);
    return $reflection->getConstants();
  }
}

/**
 * CommerceFedExDropoffType
 */
class CommerceFedExDropoffType extends CommerceFedExType {
  const BUSINESS_SERVICE_CENTER = 'Business Service Center';
  const DROP_BOX = 'Drop Box';
  const REGULAR_PICKUP = 'Regular Pickup';
  const STATION = 'Station';

  public static function getAllAssoc() {
    return parent::getAllAssoc(__CLASS__);
  }

  public static function getDefault() {
    return 'REGULAR_PICKUP';
  }
}

/**
 * CommerceFedExServiceType
 */
class CommerceFedExServiceType extends CommerceFedExType {
  const FEDEX_2_DAY = 'FedEx 2 Day';
  const FEDEX_EXPRESS_SAVER = 'FedEx Express Saver';
  const FEDEX_GROUND = 'FedEx Ground';
  const FIRST_OVERNIGHT = 'First Overnight';
  const GROUND_HOME_DELIVERY = 'Ground Home Delivery';
  const PRIORITY_OVERNIGHT = 'Priority Overnight';
  const STANDARD_OVERNIGHT = 'Standard Overnight';

  public static function getAllAssoc() {
    return parent::getAllAssoc(__CLASS__);
  }

  public static function getDefault() {
    return 'GROUND_HOME_DELIVERY';
  }
}

/**
 * CommerceFedExInput
 */
class CommerceFedExInput {
  public function __construct($properties = array()) {
    $this->apply($properties);
  }

  private function apply($properties) {
    foreach ($properties as $name => $value) {
      $name = ((is_array($value)) ? $name : $value);
      $value = (($name == $value) ? NULL : new self($value));
      $this->{$name} = $value;
    }
  }
}

/**
 * CommerceFedExShipmentCreateInput
 */
final class CommerceFedExShipmentCreateInput extends CommerceFedExInput {
  public function __construct() {
    $properties = array(
      'DropoffType',
      'ServiceType',
    );

    $properties['Shipper'] = array(
      'Contact' => array(
        'CompanyName',
        'PersonName',
        'PhoneNumber',
      ),
      'Address' => array(
        'StreetLine1',
        'StreetLine2',
        'City',
        'StateOrProvinceCode',
        'PostalCode',
        'CountryCode',
      ),
    );

    $properties['Recipient'] = array(
      'Contact' => array(
        'PersonName',
        'PhoneNumber',
      ),
      'Address' => array(
        'StreetLine1',
        'StreetLine2',
        'City',
        'StateOrProvinceCode',
        'PostalCode',
        'CountryCode',
      ),
    );

    parent::__construct($properties);
  }
}

/**
 * CommerceFedExShipmentDeleteInput
 */
final class CommerceFedExShipmentDeleteInput extends CommerceFedExInput {
  public function __construct() {
    $properties = array();

    $properties['TrackingId'] = array(
      'TrackingIdType',
      'TrackingNumber',
    );

    parent::__construct($properties);
  }
}

/**
 * CommerceFedExTrackerTrackInput
 */
final class CommerceFedExTrackerTrackInput extends CommerceFedExInput {
  public function __construct() {
    $properties = array();

    $properties['PackageIdentifier'] = array(
      'Value',
      'Type',
    );

    parent::__construct($properties);
  }
}

/**
 * CommerceFedExRequest
 */
class CommerceFedExRequest {
  protected $client;
  protected $request;

  public function __construct() {
    $this->request = array();

    $this->request['WebAuthenticationDetail'] = array(
    	'UserCredential' => array(
    		'Key' => variable_get('commerce_fedex_developer_key'),
    		'Password' => variable_get('commerce_fedex_developer_password'),
    	)
    );

    $this->request['ClientDetail'] = array(
    	'AccountNumber' => variable_get('commerce_fedex_account_number'),
    	'MeterNumber' => variable_get('commerce_fedex_meter_number'),
    );

    $this->request['TransactionDetail'] = array(
      'CustomerTransactionId' => uniqid(),
    );
  }

  protected final function response($response) {
    $map = array(
      'FAILURE' => 'error',
      'ERROR' => 'error',
      'WARNING' => 'warning',
      'NOTE' => 'status',
    );

    $set_message = NULL;
    $success = NULL;

    switch ($response->HighestSeverity) {
      case 'FAILURE':
      case 'ERROR':
        $set_message = TRUE;
        $success = FALSE;
        break;
      case 'WARNING':
      case 'NOTE':
        $set_message = TRUE;
        $success = TRUE;
        break;
      default:
        $set_message = FALSE;
        $success = TRUE;
        break;
    }

    if ($set_message) {
      $notifications = $response->Notifications;

      if (!is_array($notifications)) {
        $notifications = array($notifications);
      }

      foreach ($notifications as $notification) {
        drupal_set_message($notification->Message, $map[$notification->Severity], FALSE);
      }
    }

    return (($success) ? $response : $success);
  }
}

/**
 * CommerceFedExShipment
 */
final class CommerceFedExShipment extends CommerceFedExRequest {
  public function __construct() {
    parent::__construct();

    $this->request['Version'] = array(
    	'ServiceId' => 'ship',
    	'Major' => 10,
    	'Intermediate' => '0',
    	'Minor' => '0',
    );

    $this->client = new SoapClient(COMMERCE_FEDEX_WSDL_LOCATION . '/ShipService_v10.wsdl');
  }

  public function create(CommerceFedExShipmentCreateInput $input) {
  	$this->request['RequestedShipment'] = array(
      'ShipTimestamp' => date('c'),
      'DropoffType' => $input->DropoffType,
      'ServiceType' => $input->ServiceType,
      'PackagingType' => 'YOUR_PACKAGING',
      'RateRequestTypes' => array('ACCOUNT'),
      'PackageCount' => 1,
      'PackageDetail' => 'INDIVIDUAL_PACKAGES',
    );
    $requestedShipment = &$this->request['RequestedShipment'];

    $requestedShipment['Shipper'] = array(
      'Contact' => array(
        'CompanyName' => $input->Shipper->Contact->CompanyName,
        'PersonName' => $input->Shipper->Contact->PersonName,
        'PhoneNumber' => $input->Shipper->Contact->PhoneNumber,
      ),
      'Address' => array(
        'StreetLines' => array(
          $input->Shipper->Address->StreetLine1,
          $input->Shipper->Address->StreetLine2,
        ),
        'City' => $input->Shipper->Address->City,
        'StateOrProvinceCode' => $input->Shipper->Address->StateOrProvinceCode,
        'PostalCode' => $input->Shipper->Address->PostalCode,
        'CountryCode' => $input->Shipper->Address->CountryCode,
      )
    );

    $requestedShipment['Recipient'] = array(
      'Contact' => array(
        'PersonName' => $input->Recipient->Contact->PersonName,
        'PhoneNumber' => $input->Recipient->Contact->PhoneNumber,
      ),
      'Address' => array(
        'StreetLines' => array(
          $input->Recipient->Address->StreetLine1,
          $input->Recipient->Address->StreetLine2,
        ),
        'City' => $input->Recipient->Address->City,
        'StateOrProvinceCode' => $input->Recipient->Address->StateOrProvinceCode,
        'PostalCode' => $input->Recipient->Address->PostalCode,
        'CountryCode' => $input->Recipient->Address->CountryCode,
        'Residential' => TRUE,
      )
    );

    $requestedShipment['ShippingChargesPayment'] = array(
      'PaymentType' => 'SENDER',
      'Payor' => array(
        'AccountNumber' => variable_get('commerce_fedex_account_number'),
        'CountryCode' => $input->Shipper->Address->CountryCode,
      )
    );

    $requestedShipment['LabelSpecification'] = array(
      'LabelFormatType' => 'COMMON2D',
      'ImageType' => 'PDF',
      'LabelStockType' => 'PAPER_7X4.75',
    );

    $requestedShipment['RequestedPackageLineItems'] = array(
      array(
        'SequenceNumber' => 1,
        'Weight' => array(
          'Value' => 1.0,
          'Units' => 'LB',
        ),
        'Dimensions' => array(
          'Length' => 5,
          'Width' => 5,
          'Height' => 5,
          'Units' => 'IN'
        ),
      ),
    );

    if (!($response = parent::response($this->client->processShipment($this->request)))) {
      return $response;
    }

    return (object) array(
      'tracking_type' => $response->CompletedShipmentDetail->CompletedPackageDetails->TrackingIds->TrackingIdType,
      'tracking_number' => $response->CompletedShipmentDetail->CompletedPackageDetails->TrackingIds->TrackingNumber,
      'label_data' => $response->CompletedShipmentDetail->CompletedPackageDetails->Label->Parts->Image,
    );
  }

  public function delete(CommerceFedExShipmentDeleteInput $input) {
    $this->request['TrackingId'] = array(
      'TrackingIdType' => 'GROUND',
      'TrackingNumber' => $input->TrackingId->TrackingNumber,
    );

    $this->request['DeletionControl'] = 'DELETE_ONE_PACKAGE';

    return parent::response($this->client->deleteShipment($this->request));
  }
}

/**
 * CommerceFedExTracker
 */
final class CommerceFedExTracker extends CommerceFedExRequest {
  public function __construct() {
    parent::__construct();

    $this->request['Version'] = array(
    	'ServiceId' => 'trck',
    	'Major' => 5,
    	'Intermediate' => '0',
    	'Minor' => '0',
    );

    $this->client = new SoapClient(COMMERCE_FEDEX_WSDL_LOCATION . '/TrackService_v5.wsdl');
  }

  public function track(CommerceFedExTrackerTrackInput $input) {
    $this->request['PackageIdentifier'] = array(
      'Value' => $input->PackageIdentifier->Value,
      'Type' => 'TRACKING_NUMBER_OR_DOORTAG',
    );

    if (!($response = parent::response($this->client->track($this->request)))) {
      return $response;
    }

    return (object) array(
      'delivered' => (isset($response->TrackDetail) && !empty($response->TrackDetail->ActualDeliveryTimestamp)),
      'events' => ((isset($response->TrackDetail) && !empty($response->TrackDetail->Events)) ? $response->TrackDetail->Events : NULL),
    );
  }
}

app.xml.request = function()
{
	var public =
	{
		add : function(lineObject)
		{ private.add(lineObject); },
		reset : function()
		{ private.reset(); },
		toString : function()
		{ return private.toString(); }
	};

	var private = {};

	private.defaultValue = {};

	private.defaultValue.value = '';

	private.defaultValue.constraint =
	{
		hazardous : private.defaultValue.value,
		scac : private.defaultValue.value
	};

	private.defaultValue.event =
	{
		drop :
		{
			city : private.defaultValue.value,
			country : private.defaultValue.value,
			state : private.defaultValue.value,
			zip : private.defaultValue.value
		},
		pickUp :
		{
			city : private.defaultValue.value,
			country : private.defaultValue.value,
			date : private.defaultValue.value,
			state : private.defaultValue.value,
			zip : private.defaultValue.value
		}
	};

	private.defaultValue.item =
	{
		array : [],
		object :
		{
			freightClass : null,
			weight : null
		}
	};

	private.constraint = private.defaultValue.constraint;
	private.event = private.defaultValue.event;
	private.item = private.defaultValue.item.array;

	private.add = function(lineObject)
	{
		private.constraint.hazardous = lineObject.hazardous;
		private.constraint.scac = lineObject.scac;
		private.event.drop.city = lineObject.destinationCity;
		private.event.drop.country = lineObject.destinationCountry;
		private.event.drop.state = lineObject.destinationState;
		private.event.drop.zip = lineObject.destinationZip;
		private.event.pickUp.city = lineObject.destinationCity;
		private.event.pickUp.country = lineObject.destinationCountry;
		private.event.pickUp.date = lineObject.pickUpDate;
		private.event.pickUp.state = lineObject.destinationState;
		private.event.pickUp.zip = lineObject.destinationZip;

		private.item.push
		(
			Ext.apply
			(
				private.defaultValue.item.object,
				{
					freightClass : lineObject.freightClass,
					weight : lineObject.weight
				}
			)
		);
	};

	private.reset = function()
	{
		private.constraint = private.defaultValue.constraint;
		private.event = private.defaultValue.event;
		private.item = private.defaultValue.item.array;
	};

	private.toString = function()
	{
		var array = [];

		array.push('<?xml version="1.0"?>');
		array.push('<RateRequest>');
		array.push('<Constraints>');

		if (app.index.getValue('useScac'))
		{ array.push(String.format('<Carrier scac="{0}" />', private.constraint.scac)); }
		else
		{ array.push('<Carrier />'); }

		array.push('<Contract />');

		if (app.index.getValue('mode').length)
		{ array.push(String.format('<Mode>{0}</Mode>', app.index.getValue('mode'))); }
		else
		{ array.push('<Mode />'); }

		if (private.constraint.hazardous)
		{
			array.push('<ServiceFlags>');
			array.push('<ServiceFlag code="HAZ1">HazMat</ServiceFlag>');
			array.push('</ServiceFlags>');
		}
		else
		{ array.push('<ServiceFlags />'); }

		array.push('</Constraints>');
		array.push('<Events>');
		array.push(String.format('<Event sequence="1" type="Pickup" date="{0} 16:00">', private.event.pickUp.date));
		array.push('<Location>');
		array.push(String.format('<City>{0}</City>', private.event.pickUp.city));
		array.push(String.format('<Country>{0}</Country>', private.event.pickUp.country));
		array.push(String.format('<State>{0}</State>', private.event.pickUp.state));
		array.push(String.format('<Zip>{0}</Zip>', private.event.pickUp.zip));
		array.push('</Location>');
		array.push('</Event>');
		array.push(String.format('<Event sequence="2" type="Drop" date="{0} 16:20">', private.event.pickUp.date));
		array.push('<Location>');
		array.push(String.format('<City>{0}</City>', private.event.drop.city));
		array.push(String.format('<Country>{0}</Country>', private.event.drop.country));
		array.push(String.format('<State>{0}</State>', private.event.drop.state));
		array.push(String.format('<Zip>{0}</Zip>', private.event.drop.zip));
		array.push('</Location>');
		array.push('</Event>');
		array.push('</Events>');
		array.push('<Items>');

		for (var index = 0; index < private.item.length; index++)
		{
			array.push(String.format('<Item sequence="{0}" freightClass="{1}">', (index + 1), private.item[index].freightClass));
			array.push(String.format('<Dimensions length="{0}" width="{0}" height="{0}" units="in" />', ((app.index.getValue('mode').toLowerCase() == 'truck') ? 96 : 0)));
			array.push(String.format('<Weight units="lbs">{0}</Weight>', private.item[index].weight));
			array.push('</Item>');
		}

		array.push('</Items>');
		array.push('</RateRequest>');

		return array.join('');
	};

	return public;
}();
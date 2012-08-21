app.xml.response = function()
{
	var public =
	{
		process : function(response)
		{ return private.process(response); }
	};

	var private = {};

	private.process = function(response)
	{
		var data =
		{
			carrier : '',
			carrierCharge : 0.0,
			contractId : '',
			contractName : '',
			distance : 0.0,
			error : '',
			transitTime : 0.0
		};

		if (response.status.code != 200)
		{
			data.error = response.status.text;

			return data;
		}

		var domParser = new DOMParser();

		var document = domParser.parseFromString(response.text, 'text/xml');

		if (parseInt(document.getElementsByTagName('StatusCode')[0].textContent, 10))
		{
			data.error = document.getElementsByTagName('StatusMessage')[0].textContent.toString().trim();

			return data;
		}

		var priceSheet = document.getElementsByTagName('PriceSheet');

		if (!priceSheet.length)
		{
			data.error = 'Rating process completed without errors, but returned zero results.';

			return data;
		}

		var flag = 0;

		for (var index = 0; index < priceSheet.length; index++)
		{
			var indexPriceSheet = priceSheet[index];

			var indexData =
			{
				carrier : indexPriceSheet.getElementsByTagName('CarrierName')[0].textContent.toString().trim(),
				carrierCharge : parseFloat(indexPriceSheet.getElementsByTagName('Total')[0].textContent.toString().trim()),
				contractId : indexPriceSheet.getElementsByTagName('ContractId')[0].textContent.toString().trim(),
				contractName : indexPriceSheet.getElementsByTagName('ContractName')[0].textContent.toString().trim(),
				distance : parseFloat(indexPriceSheet.getElementsByTagName('Distance')[0].textContent.toString().trim()),
				transitTime : parseFloat(indexPriceSheet.getElementsByTagName('ServiceDays')[0].textContent.toString().trim())
			};

			if (app.index.getValue('contractId').length)
			{
				if ((String.format('({0})', app.index.getValue('contractId')) == indexData.contractId))
				{
					flag = 1;

					data = Ext.apply(data, indexData);

					break;
				}

				continue;
			}

			if (!flag)
			{
				flag = 1;

				data = Ext.apply(data, indexData);

				continue;
			}

			if (indexData.carrierCharge < data.carrierCharge)
			{
				flag = 1;

				data = Ext.apply(data, indexData);
			}
		}

		if (app.index.getValue('contractId').length && !flag)
		{
			data.error = 'Rating process completed without errors, but returned zero contract matching results.';

			return data;
		}

		return data;
	};

	return public;
}();
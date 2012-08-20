ADF.UI.NoteView = function(config) {
	var 
		local = {
			defaults : { backgroundColor : '#' + config.brand.logo.background },
			config : config,
			layout : {
				form : { items : [] }
			},
			currentData : null
		},
		self;
		
	function constructor() {
		local.layout.view = Ti.UI.createView(ADF.apply(local.defaults, local.config));
		var buttonBarCfg = [{ title : 'Email Notes' },{ title : 'Close' }];
		if (config.branch) {
			buttonBarCfg.splice(0, 0, { title : 'New Note' });
		}
		local.layout.toolbar = Ti.UI.createButtonBar({
			height : 44,
			backgroundColor : '#' + local.config.brand.toolbar.background.color,
			color : '#' + local.config.brand.toolbar.font.color,
			top : 0,
			style : Titanium.UI.iPhone.SystemButtonStyle.BAR,
			backgroundSelectedColor : '#' + local.config.brand.logo.background,
			labels : buttonBarCfg
		});
		local.layout.toolbar.addEventListener('click', function(event) {
			if (event.source.labels[event.index].title == 'New Note') {
				self.openForm();
			}
			if (event.source.labels[event.index].title == 'Close') {
				config.controller.hideNoteViewWindow();
			}
			if (event.source.labels[event.index].title == 'Email Notes') {
				self.openEmailDialog();
			}
		});
		local.layout.headerView = Ti.UI.createView({
			top : 44,
			height : 44,
			backgroundColor : '#' + local.config.brand.toolbar.background.color,
			color : '#' + local.config.brand.toolbar.font.color
		});
		local.layout.headerView.add(Ti.UI.createLabel({
			height : 44,
			top : 10,
			bottom : 10,
			left : 10,
			text : 'Date',
			color : '#' + local.config.brand.toolbar.font.color,
			width : 60
		}));
		local.layout.headerView.add(Ti.UI.createLabel({
			height : 'auto',
			top : 10,
			bottom : 10,
			left : 110,
			text : 'Action',
			color : '#' + local.config.brand.toolbar.font.color,
			width : 200
		}));
		local.layout.headerView.add(Ti.UI.createLabel({
			height : 'auto',
			top : 10,
			bottom : 10,
			left : 320,
			text : 'Note',
			color : '#' + local.config.brand.toolbar.font.color,
			width : 300
		}));
		
		local.layout.list = Ti.UI.createTableView({
			top : 88,
			layout : 'vertical',
			backgroundColor : '#' + local.config.brand.navigation.background.color,
			editable : config.branch ? true : false
		});
		local.layout.list.addEventListener('delete', function(event) {
			self.deleteNote(event.index);
		});
		
		local.layout.view.add(local.layout.toolbar);
		local.layout.view.add(local.layout.headerView);
		local.layout.view.add(local.layout.list);
		
		local.layout.formWindow = Ti.UI.iPad.createPopover({
			title : 'New Note',
			backgroundColor : '#' + local.config.brand.navigation.background.color,
			height : 500,
			width : 600
		});
		local.layout.formWindow.addEventListener('show', function() {
			local.layout.formAction.focus();
		});
		local.layout.formView = Ti.UI.createView({
			backgroundColor : '#' + local.config.brand.navigation.background.color
		});	
		local.layout.formWindow.add(local.layout.formView);
		local.layout.formWindowLeftButton = Ti.UI.createButton({
			title : 'Save',
			style : Titanium.UI.iPhone.SystemButton.SAVE
		});
		local.layout.formWindow.leftNavButton = local.layout.formWindowLeftButton;
		local.layout.formWindowLeftButton.addEventListener('click', function() {
			self.save();
		});
		local.layout.formWindowRightButton = Ti.UI.createButton({
			title : 'Cancel',
			style : Titanium.UI.iPhone.SystemButton.CANCEL
		});
		local.layout.formWindow.rightNavButton = local.layout.formWindowRightButton;
		local.layout.formWindowRightButton.addEventListener('click', function() {
			self.closeForm();
		});
		
		local.layout.formView.add(Ti.UI.createLabel({
			text : 'Date ',
			textAlign : 'right',
			height : 40,
			width : 120,
			top : 10,
			left : 10
		}));
		local.layout.formDate = Ti.UI.createTextField({
			height : 40,
			top : 10,
			left : 140,
			right : 20,
			borderStyle:Titanium.UI.INPUT_BORDERSTYLE_ROUNDED
		});
		local.layout.form.items.push(local.layout.formDate);
		local.layout.formView.add(local.layout.formDate);
		local.layout.datePicker = Ti.UI.createPicker({
			value : new Date(),
			type : Titanium.UI.PICKER_TYPE_DATE
		});
		local.layout.datePopover = Ti.UI.iPad.createPopover({
			height : 200,
			width : 300
		});
		local.layout.datePopover.add(local.layout.datePicker);
		local.layout.formDate.addEventListener('focus', function() {
			local.layout.datePopover.show({ view : local.layout.formDate });
		});
		local.layout.formDate.addEventListener('blur', function() {
			if (!local.layout.formDate.value.length) {
				local.layout.formDate.value = ADF.Date.format(local.layout.datePicker.value, 'shortDate');
			}
		});
		local.layout.datePicker.addEventListener('change', function() {
			local.layout.formDate.value = ADF.Date.format(local.layout.datePicker.value, 'shortDate');
		});
		
		local.layout.formView.add(Ti.UI.createLabel({
			text : 'Action Taken ',
			textAlign : 'right',
			height : 40,
			width : 120,
			top : 60,
			left : 10
		}));
		local.layout.formAction = Ti.UI.createTextField({
			value : '',
			height : 40,
			top : 60,
			left : 140,
			right : 20,
			borderStyle:Titanium.UI.INPUT_BORDERSTYLE_ROUNDED
		});
		local.layout.form.items.push(local.layout.formAction);
		local.layout.formView.add(local.layout.formAction);
		
		
		local.layout.formView.add(Ti.UI.createLabel({
			text : 'Note ',
			textAlign : 'right',
			height : 40,
			width : 120,
			top : 110,
			left : 10
		}));
		local.layout.formNote = Ti.UI.createTextArea({
			value : '',
			height : 340,
			top : 110,
			left : 140,
			right : 20,    
			borderWidth : 2,
    		borderColor : '#' + local.config.brand.logo.background,
    		borderRadius : 10,
    		font : { fontSize : 16 }
		});
		local.layout.form.items.push(local.layout.formNote);
		local.layout.formView.add(local.layout.formNote);
	};
	
	constructor.call(this);

	return {
		view : local.layout.view,
		init : function() {
			self = this;
			return self;
		},
		openForm : function(values) {
			self.clearForm();
			self.loadForm(values || [ADF.Date.format(new Date(), 'shortDate')]);
			// local.layout.formWindow.open({ transition : Titanium.UI.iPhone.AnimationStyle.FLIP_FROM_LEFT });
			local.layout.formWindow.show({
				view : local.layout.toolbar,
				animated : true
			});
		},
		closeForm : function(values) {
			// local.layout.formWindow.close();
			local.layout.formWindow.hide();
		},
		clearForm : function() {
			ADF.each(local.layout.form.items, function(itm, ix) {
				itm.value = '';
			});
		},
		loadForm : function(values) {
			ADF.each(local.layout.form.items, function(itm, ix) {
				itm.value = values[ix];
			});
		},
		loadList : function(data) {
			var row, dateLabel, actionLabel, noteLabel;
			
			local.layout.list.setData([]);
			ADF.each(data, function(record, index) {
				row = Ti.UI.createTableViewRow({
					height : 'auto',
					_data : record,
					// rowData : record,
					selectedBackgroundColor : '#' + local.config.brand.logo.background,
					selectedColor : '#' + local.config.brand.navigation.background.color
				});	
				dateLabel = Ti.UI.createLabel({
					height : 'auto',
					top : (config.branch ? 10 : 34),
					bottom : 10,
					left : 10,
					text : record.date,
					width : 60
				});
				row.add(dateLabel);
				actionLabel = Ti.UI.createLabel({
					height : 'auto',
					top : (config.branch ? 10 : 34),
					bottom : 10,
					left : 110,
					text : record.action,
					width : 200
				});
				row.add(actionLabel);
				noteLabel = Ti.UI.createLabel({
					height : 'auto',
					top : (config.branch ? 10 : 34),
					bottom : 10,
					left : 320,
					text : record.note,
					width : 380
				});
				if (!config.branch) {
					var crumbLabel = Ti.UI.createLabel({
						top : 0,
						height : 24,
						left : 0,
						right : 0,
						font : { fontSize : 12},
						text : config.controller.getBreadCrumb(record.key, ''),
						color : '#ffffff',
						backgroundColor : '#000000',
						opacity : 0.75
					});
					row.add(crumbLabel);
					// row.hasChild = true;
					// config.controller.addTableRowClick(local.layout.list, config.controller.getBranchCollation().get(record.key));
				}
				row.add(noteLabel);
				local.layout.list.appendRow(row);
			});
			local.currentData = data;
		},
		save : function() {
			var filePath = Ti.Filesystem.applicationDataDirectory + Ti.Filesystem.separator + config.branch.key + '.json';
			var file = Ti.Filesystem.getFile(filePath);
			var note = { date : local.layout.formDate.value, action : local.layout.formAction.value, note : local.layout.formNote.value };
			var json;
			if (file.exists()) {
				json = JSON.parse(file.read().text);
				json.note.push(note);
			}
			else {
				json = { note : [note] };
				file.createFile();
			}
			file.write(JSON.stringify(json));
			self.closeForm();
			self.loadList(json.note);
			self.updateNoteCountLabel();
		},
		deleteNote : function(index) {
			var filePath = Ti.Filesystem.applicationDataDirectory + Ti.Filesystem.separator + config.branch.key + '.json';
			var file = Ti.Filesystem.getFile(filePath);
			var note = { date : local.layout.formDate.value, action : local.layout.formAction.value, note : local.layout.formNote.value };
			var json;
			
			json = JSON.parse(file.read().text);
			json.note.splice(index, 1);
			file.write(JSON.stringify(json));
			self.closeForm();
			self.loadList(json.note);
			self.updateNoteCountLabel();
		},
		openEmailDialog : function() {
			var emailDialog = Titanium.UI.createEmailDialog(),
				html = {};
			
			if (!emailDialog.isSupported()) {
				alert('There is no email account currently active.');
				return;
			}
			html.header = '<br><br><br><br><h1>' + ((config.branch) ? config.branch.title : 'All Notes') + '</h1><table style="border-collapse: collapse; width: 100%;"><tr style="background: black; color: white;"><th>Date</th><th>Action</th><th>Note</th></tr>';
			html.row = '<tr><td style="border: solid 1px black;">{date}</td><td style="border: solid 1px black;">{action}</td><td style="border: solid 1px black;">{note}</td></tr>';
			html.footer = '</table><br><br><br><br>';
			html.rows = [];
			
			emailDialog.subject = ((config.branch) ? 'Notes for ' + config.branch.title : 'All Notes');
			emailDialog.html = true;
			
			ADF.each(local.currentData, function(record, index) {
				html.rows.push(html.row.replace(/{date}/, record.date).replace(/{action}/, record.action).replace(/{note}/, record.note));
			});
			
			emailDialog.messageBody = html.header + html.rows.join('') + html.footer;
			
			emailDialog.open();
		},
		updateNoteCountLabel : function() {
			if (config.branch) {
				config.controller.updateNoteCountLabel(' Notes ({0})', config.controller.getNoteCount(config.branch.key));
			}
			else {
				config.controller.updateNoteCountLabel(' All Notes ({0})', config.controller.getTotalNoteCount());
			}
		}
	};
};

ADF.UI.createNoteView = function(config){
	return new ADF.UI.NoteView(config).init();
};
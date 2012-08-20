ADF.mgr.Layout = function() {
	var local = {
		cmpMgr : new ADF.mgr.Component(),
		root : null,
		build : function(config) {
			/*
			var cmp, 
				viewTypes = ['detailView', 'masterView', 'headerView', 'footerView'],
				stack = [],
				recurseChildren = function(children) {
					if (children) { // loop through children calling build
						ADF.each(children, function(child) {
							child.ownerCmp = cmp;
							local.build(child);
						});
					}
				},
				createContainer = function(cfg) {
					var window = Ti.UI.createWindow(cfg); 
					local.cmpMgr.add(cfg.key || ADF.util.ID(), cfg.window);
					delete cfg;
					return window;
				};
			
			config.hidden = (config.hidden == 'undefined') ? true : config.hidden;
			config.key = config.key || ADF.util.ID();
			
			if (config.container) { // create container first
				config.window = createContainer(config.container);
			}
			
			if (config.views) { // loop through views and create
				ADF.each(config.views, function(view) {
					if (view.container) { // create container first
						view.window = createContainer(view.container);
					}
					eval('config[view.viewType] =' + local.createMethodName(view) + '(' + JSON.stringify(view) + ');');
					local.cmpMgr.add(view.key || ADF.util.ID(), config[view.viewType]);
					//recurseChildren(view.children);
				});
				delete config.views;
			}
			
			if (config.stack) { // create stack items
				ADF.each(config.stack, function(item) {
					eval('stack.push(' + local.createMethodName(view) + '(' + JSON.stringify(view) + '));');
					local.cmpMgr.add(item.key || ADF.util.ID(), stack[stack.length - 1]);
				});
				delete config.stack;
			}
			
			eval('var cmp = ' + local.createMethodName(config) + '(' + JSON.stringify(config) + ');');
			
			if (config.ownerCmp && config.ownerCmp.add) { // add cmp to ownerCmp
				config.ownerCmp.add(cmp);
			}
			
			local.cmpMgr.add(config.key, cmp);
			
			recurseChildren(config.children);
			*/
			/*eval('var cmp = ' + local.createMethodName(config) + '(' + JSON.stringify(config) + ');');
			
			ADF.iterate(config.views, function(key, value) {
				eval('viewCmps.push(' + local.createMethodName(config.views[key]) + '(' + JSON.stringify(config.views[key]) + '));');
				local.cmpMgr.add(config.views[key].key || ADF.util.ID(), viewCmps[viewCmps.length - 1]);
			});

			if (!local.root) { local.root =  cmp; }
			
			local.cmpMgr.add(config.key, cmp);
			
			if (config.ownerCmp && config.ownerCmp.add) {
				config.ownerCmp.add(cmp);
			}
			
			if (!config.hidden && cmp.open) {
				cmp.open();
			}
			
			if (config.child) {
				config.child.ownerCmp = cmp;
				local.build(config.child);
			}
			
			return cmp;*/
			


/*
			ADF.LayoutMgr.add({
				component : 'Ti.UI.iPad.SplitWindow', 
				key : 'splitWindow', 
				backgroundColor:"#fff",
				title : "My App",
				views : [{
					component : 'Ti.UI.iPhone.createNavigationGroup',
					viewType : 'masterView',
					key : 'nav',
					container : { title : 'Navigation' },
					stack : [{
						component : 'Ti.UI.Window',
						key : 'window1',
						title : 'Window 1'
					}]
				},{
					component : 'Ti.UI.View',
					viewType : 'detailView',
					title : 'Detail View',
					children : [{
						component : 'Ti.UI.Label',
						text : 'Label 1'	
					},{
						component : 'Ti.UI.Label',
						text : 'Label 2'	
					}]
				}] 
			});
*/
		},
		createMethodName : function(config) {
			var array = config.component.split('.');
			array[array.length - 1] = 'create' + array[array.length - 1];
			return array.join('.');
		}
	};
	
	return {
		add : function(config) {
			local.build(config);
			return this;
		},
		get : function(key) {
			return local.cmpMgr.get(key);
		},
		getRoot : function() {
			return local.root;
		},
		remove : function(key) { // TODO: add ability to pass in object
			var cmp = local.cmpMgr.get(key);
			if (cmp && cmp.ownerCmp) {
				cmp.ownerCmp.remove(cmp);
				return true;
			}
			return false;
		}
	};
};
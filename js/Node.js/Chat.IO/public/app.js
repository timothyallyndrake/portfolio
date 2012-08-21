Ext.require([
    'Ext.tab.*',
    'Ext.window.*',
    'Ext.tip.*',
    'Ext.layout.container.Border'
]);

Ext.onReady(function(){
    var room;

    if (!room) {
        room = Ext.create('widget.window', {
            title: 'Chat.IO',
            closable: true,
            closeAction: 'hide',
            width: 600,
            minWidth: 350,
            height: 350,
            layout: {
                type: 'border',
                padding: 5
            },
            items: [{
                region: 'center',
                items: [{
                    xtype: 'textfield',
                    name: 'message',
                }]
            }]
        });
    }


});
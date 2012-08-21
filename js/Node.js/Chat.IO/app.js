var express = require('express');
var app = express();

app.use(express.static(__dirname + '/public'));
//app.use(express.cookieParser('1337'));
//app.use(express.session());

app.set('views', __dirname + '/views');
app.set('view engine', 'jade');


app.get('/', function(req, res) {
    res.render('app');
});

app.listen(8080);
console.log('listening on port 8080...');
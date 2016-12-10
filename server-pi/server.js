var express = require('express')
var serveStatic = require('serve-static')
var app = express();    
var i2c = require('i2c');
var address = 0x04;
var wire = new i2c(address, {device: '/dev/i2c-1'}); // point to your i2c address, debug provides REPL interface

app.use(serveStatic('/var/www/html')).listen(80, function(){
 console.log('Server running on 80...');
 console.log(__dirname);
});

function runscript(relayNumber, state)
{
 wire.writeBytes(1, [relayNumber, state], function(err)
 {
  if (err)
  {
   console.log('senderr ' + err);
  }
 });
 wire.readByte(function(err, val){
  if (err)
  {
   console.log('err ' + err);
  }
 });
 var result = false;
 return result;
}

function getSwitchId(switchName)
{
 if (switchName == "livingroom")
 {
  return 0;
 }
 else if (switchName == "guestroom")
 {
  return 2;
 }
 else if (switchName == "frontdoor")
 {
  return 1;
 }
 else if (switchName == "hallway")
 {
  return 3;
 }
 else if (switchName == "backdoor")
 {
  return 4;
 }
 else if(switchName == "office")
 {
  return 5;
 }
 else if(switchName == "bedroom")
 {
  return 6;
 }
 return 0;
}

function getSwitchState(checkBoxVal)
{
 if (checkBoxVal == "true")
 {
  return 1;
 }
 return 0;
}

var bodyParser = require('body-parser');
app.use(bodyParser.urlencoded({
    extended: true
}));
app.use(bodyParser.json());
app.post('/switchflip', function(req, res){
    console.log('got post ' + req.body.switch + ' ' + req.body.state);
    runscript(getSwitchId(req.body.switch), getSwitchState(req.body.state));
    res.writeHead(200, {'Content-Type': 'text/html'});
    res.end('post received');
});

//Required modules
const express = require('express');
const es6Renderer = require('express-es6-template-engine');
const path = require('path');
const bodyParser = require('body-parser');
const fs = require('fs');
const fileName = './datajson.json';
//Initialization
const app = express();


//parsing light data
var urlencodedParser = bodyParser.urlencoded({ extended: false });


//Loading the es6 engine
app.engine('html', es6Renderer);
app.set('views', 'html');
app.set('view engine', 'html');

//index route
app.get('/', (req, res) => {
  res.render('index');
});

// post request from lights1
app.post('/lights1',urlencodedParser, (req, res) => {
  let sensitivityData = (req.body.sensitivity); // accessing the sensitivity part;
  let thresholdData = (req.body.threshold);
  let stateData = (req.body.range);
  fs.readFile(fileName, (errRead, content) => {
    if (errRead) throw errRead;
    let data = JSON.parse(content);
    // Change the "sensor_id" of the first item in "sensor_data"
    data.sensor_data[0].sensor_state = stateData;
    data.sensor_data[0].sensor_sensitivity = sensitivityData ;
    data.sensor_data[0].sensor_threshold = thresholdData ;
    // Write back new JSON
    fs.writeFile(fileName, JSON.stringify(data, null, 3), errWrite => {
      if (errWrite) throw errWrite;
      console.log('New data has been saved');
    });
  });
  console.log("success in writing");
  res.status(204).send();
});

// post request from lights2
app.post('/lights2',urlencodedParser, (req, res) => {
  let sensitivityData = (req.body.sensitivity); // accessing the sensitivity part;
  let thresholdData = (req.body.threshold);
  let stateData = (req.body.range);
  fs.readFile(fileName, (errRead, content) => {
    if (errRead) throw errRead;
    let data = JSON.parse(content);
    // Change the "sensor_id" of the first item in "sensor_data"
    data.sensor_data[1].sensor_state = stateData;
    data.sensor_data[1].sensor_sensitivity = sensitivityData ;
    data.sensor_data[1].sensor_threshold = thresholdData ;
    // Write back new JSON
    fs.writeFile(fileName, JSON.stringify(data, null, 3), errWrite => {
      if (errWrite) throw errWrite;
      console.log('New data has been saved');
    });
  });
  console.log("success in writing");
  res.status(204).send(); // telling the nodejs server to not do anything and solves reloding
});

//lights page route
app.get('/lights.html', (req, res) => {

  res.render('lights');
});

app.get('*', function(req, res, next){
  res.locals.user = req.user || null;
  next();
});

//Public folder
app.use(express.static(path.join(__dirname,'public')));

 app.listen(3000, () => {
   console.log('Server start');
 });

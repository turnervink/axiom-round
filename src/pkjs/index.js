var Clay = require('pebble-clay');
var clayConfig = require('./config.json');
var clay = new Clay(clayConfig);

var xhrRequest = function (url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
  xhr.open(type, url);
  xhr.send();
};

function locationSuccess(pos) {
  // Construct URL
  console.log("Fetching weather with GPS location");
  console.log("Lat is " + pos.coords.latitude);
  console.log("Lon is " + pos.coords.longitude);
  var url = 'http://api.openweathermap.org/data/2.5/weather?lat=' + pos.coords.latitude + '&lon=' + pos.coords.longitude + '&appid=26572bd92483d703bc3cf2e160e95cd1&lang=';

  console.log('URL is ' + url);

  // Send request to OpenWeatherMap
  xhrRequest(url, 'GET',
    function(responseText) {
      console.log("Parsing JSON");

      var json = JSON.parse(responseText); // Parse JSON response

      if (!json.main) {
        var dictionary = {
          "MsgKeyError": "error",
        };
      } else {
        var temperature = Math.round(((json.main.temp - 273.15) * 1.8) + 32); // Convert from Kelvin to Fahrenheit
        console.log("Temperature in Fahrenheit is " + temperature);

        var temperaturec = Math.round(json.main.temp - 273.15); // Convert from Kelvin to Celsius
        console.log("Temperature in Celsius is " + temperaturec);

        var condcode = json.weather[0].id;
        console.log("Conditions code is " + condcode);

        // Assemble weather info into dictionary
        var dictionary = {
          "MsgKeyTemp": temperature,
          "MsgKeyTempC": temperaturec,
          "MsgKeyCondCode": condcode
        };
      }

      // Send dictionary to Pebble
      Pebble.sendAppMessage(dictionary,
        function(e) {
          console.log("App message sent containing response from weather service");
					console.log(e);
        },
        function(e) {
          console.log("Error sending info to Pebble - response received from weather service");
					console.log(e);
        }
      );
    }
  );
}

function locationError(err) {
  console.log('Error requesting location!');

  var dictionary = {
    "MsgKeyError": "error",
  };

  Pebble.sendAppMessage(dictionary,
    function(e) {
      console.log("Location error info sent to Pebble successfully!");
      console.log(e);
    },
    function(e) {
      console.log("Could not send location error info to Pebble!");
      console.log(e);
    }
  );
}

function getWeather() {
	console.log("Getting weather");
	navigator.geolocation.getCurrentPosition(
		locationSuccess,
		locationError,
		{timeout: 15000, maximumAge: 60000}
	);
}


// JS Ready
Pebble.addEventListener("ready", function(e) {
  console.log("PebbleKit JS Ready!");

	getWeather(); // Get weather on launch
});

Pebble.addEventListener("appmessage", function(e) {
  console.log("AppMessage received!");

  var msgContents = e.payload;

  if (msgContents.MsgKeyCondCode === 0) {
    console.log("Received weather update request!");
    getWeather();
  }
});

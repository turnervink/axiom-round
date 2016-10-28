/******/ (function(modules) { // webpackBootstrap
/******/ 	// The module cache
/******/ 	var installedModules = {};

/******/ 	// The require function
/******/ 	function __webpack_require__(moduleId) {

/******/ 		// Check if module is in cache
/******/ 		if(installedModules[moduleId])
/******/ 			return installedModules[moduleId].exports;

/******/ 		// Create a new module (and put it into the cache)
/******/ 		var module = installedModules[moduleId] = {
/******/ 			exports: {},
/******/ 			id: moduleId,
/******/ 			loaded: false
/******/ 		};

/******/ 		// Execute the module function
/******/ 		modules[moduleId].call(module.exports, module, module.exports, __webpack_require__);

/******/ 		// Flag the module as loaded
/******/ 		module.loaded = true;

/******/ 		// Return the exports of the module
/******/ 		return module.exports;
/******/ 	}


/******/ 	// expose the modules object (__webpack_modules__)
/******/ 	__webpack_require__.m = modules;

/******/ 	// expose the module cache
/******/ 	__webpack_require__.c = installedModules;

/******/ 	// __webpack_public_path__
/******/ 	__webpack_require__.p = "";

/******/ 	// Load entry module and return exports
/******/ 	return __webpack_require__(0);
/******/ })
/************************************************************************/
/******/ ([
/* 0 */
/***/ function(module, exports, __webpack_require__) {

	module.exports = __webpack_require__(1);


/***/ },
/* 1 */
/***/ function(module, exports) {

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
	  var url = 'http://api.openweathermap.org/data/2.5/weather?lat=' + pos.coords.latitude + '&lon=' + pos.coords.longitude + '&appid=2874bea34ea1f91820fa07af69939eea&lang=';

	  console.log('URL is ' + url);

	  // Send request to OpenWeatherMap
	  xhrRequest(url, 'GET',
	    function(responseText) {
	      console.log("Parsing JSON");

	      var json = JSON.parse(responseText); // Parse JSON response

	      if (!json.main) {
	        var dictionary = {
	          "CfgKeyWeatherError": "error",
	        };
	      } else {
	        var temperature = Math.round(((json.main.temp - 273.15) * 1.8) + 32); // Convert from Kelvin to Fahrenheit
	        console.log("Temperature in Fahrenheit is " + temperature);

	        var temperaturec = Math.round(json.main.temp - 273.15); // Convert from Kelvin to Celsius
	        console.log("Temperature in Celsius is " + temperaturec);

	        // Assemble weather info into dictionary
	        var dictionary = {
	          "MsgKeyTemp": temperature,
	          "MsgKeyTempC": temperaturec,
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


/***/ }
/******/ ]);
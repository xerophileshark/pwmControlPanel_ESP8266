# pwmControlPanel_ESP8266
A simple PWM control panel with ESP8266 boards using arduino.

## Usage
There are two habdlers:
  1. ```"/"``` handler which loads the neccesary panel html and javascript.
  2. ```"/pwm"``` handler which handles the pwm requests. **You should implement** this handler. this handler uses *POST* method to receive pwm data as ```percent``` argument.
To start the panel, simply enter the esp8266's ip address in your browser.

![1](https://user-images.githubusercontent.com/30368346/92375989-25dcf180-f117-11ea-9b16-2b57b542c90d.PNG)

The panel should look like this:

![2](https://user-images.githubusercontent.com/30368346/92376356-b1568280-f117-11ea-9b0c-2202daa078f2.PNG)

## Panel html
the panel html code:
```html
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<style>
.slidecontainer {
  width: 100%;
}

.slider {
  -webkit-appearance: none;
  width: 100%;
  height: 50px;
  background: #d3d3d3;
  outline: none;
  opacity: 0.7;
  -webkit-transition: .2s;
  transition: opacity .2s;
}

.slider:hover {
  opacity: 1;
}

.slider::-webkit-slider-thumb {
  -webkit-appearance: none;
  appearance: none;
  width: 35px;
  height: 50px;
  background: purple;
  cursor: pointer;
}

.slider::-moz-range-thumb {
  width: 35px;
  height: 50px;
  background: purple;
  cursor: pointer;
}
</style>
</head>
<body>

<h1 style="text-align: center;">PWM control panel using esp8266</h1>
<p>Drag the slider to change the PWM percentage.</p>

<div class="slidecontainer">
  <input type="range" min="0" max="100" value="0" class="slider" id="myRange">
  <p>Value: %<span id="demo"></span></p>
  <p>response: <span id="resp"></span></p>
</div>

<script>
setInterval(function(){ document.getElementById('resp').innerHTML = ''; }, 4000);

var slider = document.getElementById("myRange");
var output = document.getElementById("demo");
output.innerHTML = slider.value;

slider.oninput = function() {
  output.innerHTML = this.value;
  var xhr = new XMLHttpRequest();
  xhr.open('post', '/pwm');
  xhr.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');
  xhr.send('percent=' + this.value);
  xhr.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
	 document.getElementById('resp').visibility = 'visible';
     document.getElementById("resp").innerHTML = this.responseText;
    }
  };
}
</script>

</body>
</html>
```

This code is stringified using [this site](http://davidjwatts.com/youtube/esp8266/esp-convertHTM.html) and put in ```String html``` variable. For the sake of simplicity we didn't use file system.

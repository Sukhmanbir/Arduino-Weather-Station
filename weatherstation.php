<?php

$BASE_URL = "http://query.yahooapis.com/v1/public/yql";

$yql_query = 'select item from weather.forecast where woeid in (select woeid from geo.places(1) where text="barrie, ca")';
$yql_query_url = $BASE_URL . "?q=" . urlencode($yql_query) . "&format=json";

// Make call with cURL
$session = curl_init($yql_query_url);
curl_setopt($session, CURLOPT_RETURNTRANSFER, true);
$json = curl_exec($session);
// Convert JSON to PHP object
$phpObj = json_decode($json);

$array1 = $phpObj -> query -> results -> channel -> item;
$title = $array1 -> title;
$lat = $array1 -> lat;
$long = $array1 -> long;
$forecastArray = $array1 -> forecast;
echo "$";
echo $title;
echo "|";
echo 'Latitude ' . $lat . '';
echo "|";
echo 'Longitude ' . $long . '';
echo "|";

for ($i = 0; $i < 2; $i++) {
	foreach ($forecastArray[$i] as $x => $x_value) {
		if ($x != "code") {
			if($x == "text")
			{
				echo "Forecast " . $x_value;
			}
			else {
			echo $x . " " . $x_value;	
			}
			echo "|";
		}
	}
}
?>

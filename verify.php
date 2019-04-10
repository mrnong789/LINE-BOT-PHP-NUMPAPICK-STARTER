<?php
$access_token = 'MDogdQfR5pelyDFp24lerYcf3ehef8x/GfOt7PpFBH2HyHe/lCHMfgSaRKe3Bbc9pbc2hFKptcocK7RwR60ySMMri9pZxsskkuox7jeM5Y04YSHwA8T1wv33njtaz2ntxIOROcvUVXzmpqud8j3C0AdB04t89/1O/w1cDnyilFU=';
$url = 'https://api.line.me/v1/oauth/verify';
$headers = array('Authorization: Bearer ' . $access_token);
$ch = curl_init($url);
curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
curl_setopt($ch, CURLOPT_HTTPHEADER, $headers);
curl_setopt($ch, CURLOPT_FOLLOWLOCATION, 1);
$result = curl_exec($ch);
curl_close($ch);
echo $result;
?>
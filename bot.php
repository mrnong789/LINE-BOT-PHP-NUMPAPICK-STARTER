<?php
//  require("pub.php");
//  require("line.php");
// // Get POST body content
// $content = file_get_contents('php://input');
// // Parse JSON
// $events = json_decode($content, true);
// // Validate parsed JSON data
// if (!is_null($events['ESP'])) {
//    send_LINE($events['ESP']);
//    echo "OK";
// }
// if (!is_null($events['events'])) {
//    echo "line bot";
//    // Loop through each event
//    foreach ($events['events'] as $event) {
// 	// Reply only when message sent is in 'text' format
// 	if ($event['type'] == 'message' && $event['message']['type'] == 'text') {
// 	   // Get text sent
// 	   $text = $event['message']['text'];
// 	   // Get replyToken
// 	   $replyToken = $event['replyToken'];
//   	   // Build message to reply back
//     	$Topic = "ESP32" ;
// 	   getMqttfromlineMsg($Topic,$text);
// 	}
//     }
//  }
//  echo "OK3";








$strAccessToken = "MDogdQfR5pelyDFp24lerYcf3ehef8x/GfOt7PpFBH2HyHe/lCHMfgSaRKe3Bbc9pbc2hFKptcocK7RwR60ySMMri9pZxsskkuox7jeM5Y04YSHwA8T1wv33njtaz2ntxIOROcvUVXzmpqud8j3C0AdB04t89/1O/w1cDnyilFU=";
 
$content = file_get_contents('php://input');
$arrJson = json_decode($content, true);
 
$strUrl = "https://api.line.me/v2/bot/message/reply";
 
$arrHeader = array();
$arrHeader[] = "Content-Type: application/json";
$arrHeader[] = "Authorization: Bearer {$strAccessToken}";
 
if($arrJson['events'][0]['message']['text'] == "สวัสดี"){
  $arrPostData = array();
  $arrPostData['replyToken'] = $arrJson['events'][0]['replyToken'];
  $arrPostData['messages'][0]['type'] = "text";
  $arrPostData['messages'][0]['text'] = "สวัสดี ID คุณคือ ".$arrJson['events'][0]['source']['userId'];
}else if($arrJson['events'][0]['message']['text'] == "ชื่ออะไร"){
  $arrPostData = array();
  $arrPostData['replyToken'] = $arrJson['events'][0]['replyToken'];
  $arrPostData['messages'][0]['type'] = "text";
  $arrPostData['messages'][0]['text'] = "ฉันยังไม่มีชื่อนะ";
}else if($arrJson['events'][0]['message']['text'] == "ทำอะไรได้บ้าง"){
  $arrPostData = array();
  $arrPostData['replyToken'] = $arrJson['events'][0]['replyToken'];
  $arrPostData['messages'][0]['type'] = "text";
  $arrPostData['messages'][0]['text'] = "ฉันทำอะไรไม่ได้เลย คุณต้องสอนฉันอีกเยอะ";
}else{
  $arrPostData = array();
  $arrPostData['replyToken'] = $arrJson['events'][0]['replyToken'];
  $arrPostData['messages'][0]['type'] = "text";
  $arrPostData['messages'][0]['text'] = "ฉันไม่เข้าใจคำสั่ง";
}
 
 
$ch = curl_init();
curl_setopt($ch, CURLOPT_URL,$strUrl);
curl_setopt($ch, CURLOPT_HEADER, false);
curl_setopt($ch, CURLOPT_POST, true);
curl_setopt($ch, CURLOPT_HTTPHEADER, $arrHeader);
curl_setopt($ch, CURLOPT_POSTFIELDS, json_encode($arrPostData));
curl_setopt($ch, CURLOPT_RETURNTRANSFER,true);
curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, false);
$result = curl_exec($ch);
curl_close ($ch);
?>
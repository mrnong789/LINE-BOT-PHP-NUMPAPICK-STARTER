<?php
function send_LINE($msg){
 $access_token = 'MDogdQfR5pelyDFp24lerYcf3ehef8x/GfOt7PpFBH2HyHe/lCHMfgSaRKe3Bbc9pbc2hFKptcocK7RwR60ySMMri9pZxsskkuox7jeM5Y04YSHwA8T1wv33njtaz2ntxIOROcvUVXzmpqud8j3C0AdB04t89/1O/w1cDnyilFU=';    //PUT LINE token ID at "Channel access token (long-lived)" 
 $messages = [
        'type' => 'text',
        'text' => $msg
      ];
      // Make a POST Request to Messaging API to reply to sender
      $url = 'https://api.line.me/v2/bot/message/push';
      $data = [
        'to' => 'C4025d4f09c33c08f3f506f23eb08ddbc',         //PUT LINE ID at "Your user ID"
        'messages' => [$messages],
      ];
      $post = json_encode($data);
      $headers = array('Content-Type: application/json', 'Authorization: Bearer ' . $access_token);
      $ch = curl_init($url);
      curl_setopt($ch, CURLOPT_CUSTOMREQUEST, "POST");
      curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
      curl_setopt($ch, CURLOPT_POSTFIELDS, $post);
      curl_setopt($ch, CURLOPT_HTTPHEADER, $headers);
      curl_setopt($ch, CURLOPT_FOLLOWLOCATION, 1);
      $result = curl_exec($ch);
      curl_close($ch);
      echo $result . "\r\n"; 
}
?>
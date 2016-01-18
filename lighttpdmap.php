<?php
/**
*  
* @var
*/

$pattern = '/^(\d+\.\d+\.\d+\.\d+) (\d+\.\d+\.\d+\.\d+) (.+?) \[(.+?)\] (\d+) \"([^ ]+?) ([^ ]+?) ([^ ]+?)\" \"(.+?)\" (\d+) (.+?) (.+?) \"(.+?)\" \"(.+?)\" \"(.+?)\" (.+?) (.+?) (.+?) (\d+\.\d+\.\d+\.\d+) \"(.+)\"$/';

while($line = fgets(STDIN)) {
    $line = trim($line);
    if (empty($line)) {
        continue;
    }
    if (!preg_match($pattern, $line, $matches)) {
        continue;
    }

    $ip = $matches[1];

    $url = $matches[7];
    $arrUrl = parse_url($url);

    $preUrl = !empty($arrUrl["path"]) ? $arrUrl["path"] : '';
    $queryUrl = !empty($arrUrl["query"]) ? $arrUrl["query"] : '';

    parse_str($queryUrl, $arrQuery);

    $qt = !empty($arrQuery['qt']) ? $arrQuery['qt'] : '';
    $pn = !empty($arrQuery['pn']) ? $arrQuery['pn'] : '';


    $cookie = $matches[14];

    $arrCookie = explode(";", $cookie);
    $bduss = '';
    foreach($arrCookie as $cookieItem) {
        $cookieItem = trim($cookieItem);
        $arrItem = explode("=", $cookieItem);
        if (count($arrItem) != 2 || empty($arrItem[0])) {
            continue;
        }
        if($arrItem[0] == "BDUSS" || $arrItem[0] == "bduss") {
            $bduss = $arrItem[1];
            break;
        }
    }
    $key = $ip;
    if(!empty($bduss)) {
        $key = $bduss;
    }
    $timestamp = strtotime($matches[4]);


    $strOut = $timestamp . "\t" . $ip . "\t" . $key . "\t" . $preUrl . "\t" . $qt . "\t" . $pn . "\n";
    //fwrite(STDOUT, $strOut);
    echo $strOut;
}

?>

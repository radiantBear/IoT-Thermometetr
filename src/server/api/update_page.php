<!-- 

Returns whatever data is currently stored, regardless of whether it's stale

 -->

<?php
    // Double check that file has data
    if(filesize('./data/data.txt') > 0) {
        // Open file to display data
        $dataFile = fopen('./data/data.txt', 'r') or die('Unable to open file!');

        // Print all file contents in JSON format
        echo str_replace('&quot;', '"', htmlspecialchars(file_get_contents('./data/data.txt')));

        // Close file
        fclose($dataFile);
    }
?>
<!-- 

Accepts POST requests to update the saved data
and GET requests that return stored data if it's not stale 

-->

<?php
    // Update the file if new data was sent via POST -- GET requests to this page are requesting the stored data
    if ($_SERVER["REQUEST_METHOD"] == "POST") {
        // This means the ESP32 is sending updated data
        
        // Get current file contents -- NOTE: true arg VERY IMPORTANT for associative array
        $currentFile = json_decode(file_get_contents("./data/data.txt"), true);

        // Update each part of the array with POST data if it's there, otherwise preserve the old value
        $data = array();
        if(isset($_POST["current"])) {
            // Check if message is that data should be discarded
            if($_POST["current"] == "-") {
                $data["current"] = 0;
                $data["lastTemp"] = 0;
            } else {
                $data["current"] = intval($_POST["current"]);
                $data["lastTemp"] = time();
            }
        } else {
            $data["current"] = $currentFile["current"];
            $data["lastTemp"] = $currentFile["lastTemp"];
        }
        if(isset($_POST["minimum"])) {
            $data["minimum"] = intval($_POST["minimum"]);
        } else {
            $data["minimum"] = $currentFile["minimum"];
        }
        if(isset($_POST["maximum"])) {
            $data["maximum"] = intval($_POST["maximum"]);
        } else {
            $data["maximum"] = $currentFile["maximum"];
        }
        if(isset($_POST["fullAverage"])) {
            $data["fullAverage"] = floatval($_POST["fullAverage"]);
        } else {
            $data["fullAverage"] = $currentFile["fullAverage"];
        }
        if(isset($_POST["observations"])) {
            $data["observations"] = intval($_POST["observations"]);
        } else {
            $data["observations"] = $currentFile["observations"];
        }
        if(isset($_POST["averages"])) {
            // Check if message is that data should be discarded
            if($_POST["averages"] == "-") {
                $data["averages"] = array();
                $data["lastAvg"] = 0;
            } else {
                $data["averages"] = array_map("floatval", explode(",", $_POST["averages"]));
                $data["lastAvg"] = time();
            }
        } else {
            $data["averages"] = $currentFile["averages"];
            $data["lastAvg"] = $currentFile["lastAvg"];
        }

        // Open file for writing -- WARNING: clears current file
        $myfile = fopen("data/data.txt", "w") or die("Unable to open file!");

        // Write updated data back to file
        fwrite($myfile, json_encode($data));

        fclose($myfile);
    } else {
        // This means the ESP32 is requesting data to restore after restart

        // Get current file contents
        $dataFileContents = file_get_contents("./data/data.txt");
        $data = json_decode($dataFileContents, true);

        // Send the stored data if it's less than 5 minutes old
        if(time() - $data["lastTemp"] < 60 * 5) {
            echo str_replace("&quot;", "\"", htmlspecialchars($dataFileContents));
        }
    }
?>
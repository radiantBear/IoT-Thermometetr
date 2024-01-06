<!-- 

Builds the webpage for the thermometer owner to see the data it's collected

-->

<html>
    <head>
        <title>Sensor Measurements</title>

        <!-- Enable the default dark mode styles if browser theme is dark-->
        <meta name="color-scheme" content="dark light">

        <!-- Include page styling rules -->
        <link rel="stylesheet" href="styles.css">
    </head>

    <body>
        <!-- Title & subtitle -->
        <h1>OSU ENGR 103 Project</h1>
        <h2>Thermometer Reading</h2>

        <!-- Display the current temperature -->
        <h1 id="currentTemp">
            <!-- Embed current temperature @ page load time -->
            <?php
                // Double check that file has data
                if(filesize("data/data.txt") > 0) {
                    // Open file to display data
                    $myfile = fopen("data/data.txt", "r") or die("Unable to open file!");
                    $data = json_decode(file_get_contents("./data/data.txt"), true);

                    // Get last reading & what time it was at
                    $currentTemp = htmlspecialchars($data["current"]) . "ºF";
                    $secAgo = $data["lastTemp"];

                    // Give last temp if < 5 min old
                    if(time() - $secAgo < 60 * 5) {
                        // Print file contents
                        echo $currentTemp; 
                    } else {
                        echo "--";
                    }

                    // Close file
                    fclose($myfile);
                } else {
                    echo "--";
                }
            ?>
        </h1>
        <p>Current temperature</p>

        <!-- Display the summary information (minimum, maximum, & average temperatures) -->
        <div id="summary">
            <div id="minimum">
                <h2 id="minTxt"><?php
                    // Double check that file has data
                    if(filesize("data/data.txt") > 0) {
                        // Open file to display data
                        $myfile = fopen("data/data.txt", "r") or die("Unable to open file!");
                        $data = json_decode(file_get_contents("./data/data.txt"), true);

                        // Get last reading & what time it was at
                        $minimum = htmlspecialchars($data["minimum"]) . "ºF";

                        // Print file contents
                        echo $minimum;

                        // Close file
                        fclose($myfile);
                    } else {
                        echo "--";
                    }
                ?></h2>
                <p>Minimum&nbsp;</p>
            </div>
            <div id="average">
                <h2 id="avgTxt"><?php
                    // Double check that file has data
                    if(filesize("data/data.txt") > 0) {
                        // Open file to display data
                        $myfile = fopen("data/data.txt", "r") or die("Unable to open file!");
                        $data = json_decode(file_get_contents("./data/data.txt"), true);

                        // Get last reading & what time it was at
                        $average = htmlspecialchars($data["fullAverage"] ?? '') . "ºF";

                        // Print file contents
                        echo $average;

                        // Close file
                        fclose($myfile);
                    } else {
                        echo "--";
                    }
                ?></h2>
                <p>Average&nbsp;</p>
            </div>
            <div id="maximum">
                <h2 id="maxTxt"><?php
                    // Double check that file has data
                    if(filesize("data/data.txt") > 0) {
                        // Open file to display data
                        $myfile = fopen("data/data.txt", "r") or die("Unable to open file!");
                        $data = json_decode(file_get_contents("./data/data.txt"), true);

                        // Get last reading & what time it was at
                        $maximum = htmlspecialchars($data["maximum"]) . "ºF";

                        // Print file contents
                        echo $maximum;

                        // Close file
                        fclose($myfile);
                    } else {
                        echo "--";
                    }
                ?></h2>
                <p>Maximum&nbsp;</p>
            </div>
        </div>

        <!-- Chart of data over past 24 hrs -->
        <canvas id="tempData" style="width:750px; max-width:750px; height: 500px; max-height: 500px;"></canvas>

        <!-- Chart.js -- easy-to-use graphing library -->
        <script src="https://cdnjs.cloudflare.com/ajax/libs/Chart.js/3.9.1/chart.js"></script>

        <!-- Include this in PHP file to avoid redundant GET request -->
        <script>
            yValues = [<?php
                // Double check that file has data
                if(filesize("./data/data.txt") > 0) {
                    // Open file to display data
                    $myfile = fopen("./data/data.txt", "r") or die("Unable to open file!");
                    $data = json_decode(file_get_contents("./data/data.txt"), true);

                    // Get current list & what time Arduino last refreshed data
                    $averages = htmlspecialchars(implode(", ", $data["averages"]));
                    $avgSecAgo = $data["lastAvg"];


                    // Print averages if they're still current
                    if(time() - $avgSecAgo < 60 * 20) {
                        echo $averages;
                    }

                    // Close file
                    fclose($myfile);
                }
            ?>];
        </script>

        <!-- Make chart work; auto-refresh webpage -->
        <script src="main.js"></script>
    </body>
</html>
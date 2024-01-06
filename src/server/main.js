const ARRAY_LENGTH = 6*24; // Set the length for the data passed to Chart.js for consistent scaling

// Get system theme:
const prefersDarkScheme = window.matchMedia("(prefers-color-scheme: dark)");
let useDark = prefersDarkScheme.matches;

// If system theme changes, change graph colors
//   Event listener credit: https://stackoverflow.com/a/57795495/21684315
prefersDarkScheme.addEventListener('change', e => {
    useDark = e.matches;
    if(useDark) {
        tempChart.options.scales.x.grid.color = '#444';
        tempChart.options.scales.y.grid.color = '#444';
        tempChart.options.scales.x.grid.borderColor = '#bbb';
        tempChart.options.scales.y.grid.borderColor = '#bbb';
        tempChart.options.scales.x.title.color = '#999';
        tempChart.options.scales.y.title.color = '#999';
        tempChart.options.plugins.title.color = "#999";
    } else {
        tempChart.options.scales.x.grid.color = "rgba(0, 0, 0, 0.1)";
        tempChart.options.scales.y.grid.color = "rgba(0, 0, 0, 0.1)";
        tempChart.options.scales.x.grid.borderColor = '#444';
        tempChart.options.scales.y.grid.borderColor = '#444';
        tempChart.options.scales.x.title.color = '#666';
        tempChart.options.scales.y.title.color = '#666';
        tempChart.options.plugins.title.color = "#666";
    }
    tempChart.update();
});

// Create an array that increases by 'step' from 'start' to 'stop'
//   Function from Joel Olawanle: 
//     https://www.freecodecamp.org/news/javascript-range-create-an-array-of-numbers-with-the-from-method/
const arrayRange = (start, stop, step) =>
    Array.from(
    { length: (stop - start) / step + 1 },
    (value, index) => (start + index * step).toFixed(2)
    );

// Fill the array with blank elements (allows right-aligned line with Chart.js)
function fillArray(array) {
    while(array.length < ARRAY_LENGTH) {
        array.unshift("N/A");
    }
    return array;
}

// Fill the starting array that was created in PHP
fillArray(yValues);

// Create the chart object for Chart.js
let tempChart = new Chart("tempData", {
    type: "line",
    data: {
        labels: arrayRange(0, ARRAY_LENGTH, 1),
        datasets: [{
            borderColor: "#55f",
            data: yValues,
            fill: false,
            lineTension: 0.25,
            pointRadius: 0,
        }]
    },
    options: {
        plugins: {
            title: {
                display: true,
                text: "Averages Over Past 24 Hours",
                font: {size: 24},
                color: useDark ? '#999' : '#666'
            },
            legend: {display: false},
        },
        scales: {
            y: {
                title: {text: "Temperature", display: true, font: {size: 16}, color: useDark ? '#999' : '#666'}, 
                // min: 64, 
                // max:84, 
                ticks: {callback: function(value) {return value + "ºF"}},
                grid: {color: (useDark ? '#444' : 'rgba(0, 0, 0, 0.1)'), borderColor: (useDark ? '#bbb' : '#444')}
            },
            x: {
                title: {text: "Hours Ago", display: true, font: {size: 16}, color: useDark ? '#999' : '#666'}, 
                ticks: {maxRotation: 40, minRotation: 40, maxTicksLimit: 13, callback: function(value, index, ticks) {
                    // Format x-axis labels for "-HH:MM"
                    value = Chart.Ticks.formatters.numeric.apply(this, [value, index, ticks]);
                    return "-" + (24 - Math.round(value / 6)).toString().padStart(2, "0") + ":" + (value % 6 * 10).toString().padStart(2, "0");
                }},
                grid: {color: (useDark ? '#444' : 'rgba(0, 0, 0, 0.1)'), borderColor: (useDark ? '#bbb' : '#444')}
            },
        }
    }
});

// Update the chart with new information
function updateChart(yValues) {
    tempChart.data.datasets[0].data = yValues;
    tempChart.update();
}

// Get new information from the server
function refreshData() {
    fetch("https://web.engr.oregonstate.edu/~bairdn/data_logger/api/update_page.php", {
        method: 'GET',
    })
    .then((response) => response.json())
    .then((json) => {
        let tempSecAgo = (new Date().getTime() / 1000) - json.lastTemp;
        let avgSecAgo = (new Date().getTime() / 1000) - json.lastAvg;

        // Update w/ new data if it's still valid, otherwise show no readings
        if(tempSecAgo < 60 * 5) {
            document.getElementById("currentTemp").innerHTML = json.current + "ºF";
        } else {
            document.getElementById("currentTemp").innerHTML = "--";
        }

        // Update the summary information
        document.getElementById("minTxt").innerHTML = json.minimum + 'ºF';
        document.getElementById("avgTxt").innerHTML = (json.fullAverage ?? '') + 'ºF';
        document.getElementById("maxTxt").innerHTML = json.maximum + 'ºF';

        // Keep showing the chart if it was pretty recently updated, otherwise clear it
        if(avgSecAgo < 60 * 20) {
            updateChart(fillArray(json.averages));
        } else {
            updateChart(fillArray([])); // Update with blank array to clear chart
        }

        // Set timer for next data refresh
        //   Ideally 3 seconds after the thermometer sends new info to the server, 
        //   but no more than once every 10 sec in case there's no new data
        setTimeout(refreshData, Math.max(33 - tempSecAgo, 10) * 1000);
    });
}

// Update the page's data after 30 seconds
setTimeout(refreshData, 30 * 1000);
# Server Setup

The majority of the setup for the server will be configuring an application to serve PHP content. Apache is a solid option for this; you can find installation instructions [here](https://httpd.apache.org/docs/2.4/install.html).

Once the server application is running, copy the files under `src/server` to the file directory that the application is serving content from. 
> [!NOTE]
> The directory structure beneath the server's root directory will determine the URI of the website. For example, if you place the files under `SERVER_DIRECTORY/projects/thermometer`, you will be able to access the webpage from `http://yourdomain.com/projects/thermometer`.

That's it! Be sure to update the `/src/arduino/secrets.h` file to include the web path to the website (eg `http://yourdomain.com/projects/thermometer`) so that it can upload data properly.
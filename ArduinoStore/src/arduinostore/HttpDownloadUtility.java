package arduinostore;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.URL;
/*
 * A class that downloads a file from a URL.
 */
public class HttpDownloadUtility {
    private static final int BUFFER_SIZE = 4096;
    
    /**
     * Downloads a file from a URL
     * @param fileURL HTTP URL of the file to be downloaded
     * @param saveDir path of the directory to save the file
     * @throws IOException
     */
    public static void downloadFile(String fileURL, String saveDir) throws IOException {
        
        // Sets up the URL and HTTP connection
        URL url = new URL(fileURL);
        HttpURLConnection httpConn = (HttpURLConnection) url.openConnection();
        int responseCode = httpConn.getResponseCode();
        
        // Checks for HTTP response code
        if (responseCode == HttpURLConnection.HTTP_OK) {
            String fileName = "";
            String disposition = httpConn.getHeaderField("Content-Disposition");
            String contentType = httpConn.getContentType();
            int contentLength = httpConn.getContentLength();
        
            if (disposition != null) {
                // Extracts file name from header field
                System.out.println("Entered if(disposition != null");
                int index = disposition.indexOf('=');
                if (index > 0) {
                    fileName = disposition.substring
                                (index + 1, disposition.length());
                }
            } else {
                // Extracts file name from URL
                fileName = fileURL.substring(fileURL.lastIndexOf("/") + 1,
                       fileURL.length());
            }
            
            // Display file information to the user
            System.out.println("Content-Type = " + contentType);
            System.out.println("Content-Disposition = " + disposition);
            System.out.println("Content-Length = " + contentLength);
            System.out.println("fileName = " + fileName);
            
            // Opens input stream from the HTTP connection
            InputStream inputStream = httpConn.getInputStream();
            String saveFilePath = saveDir + File.separator + fileName.substring(1,fileName.length());
            
            // Opens an output stream to save into file
            FileOutputStream outputStream = new FileOutputStream(saveFilePath);
            
            // Read bytes and write to the file as long as there is data available
            int bytesRead = -1;
            byte[] buffer = new byte[BUFFER_SIZE];
            while ((bytesRead = inputStream.read(buffer)) != -1) {
                outputStream.write(buffer, 0, bytesRead);
            }
            
            // Close input and output stream
            outputStream.close();
            inputStream.close();
            
            // Show that file is downlaoded
            if (fileName!="filelist.txt") System.out.println("File downloaded");
        } else {
            // Show if file is not available
            System.out.println("No file to download. Server replied HTTP code: " + responseCode);
        }
        
        // Close the HTTP connection
        httpConn.disconnect();

        }
    }

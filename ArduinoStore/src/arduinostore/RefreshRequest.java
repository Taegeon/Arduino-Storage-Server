package arduinostore;

import java.io.File;
import java.io.FileOutputStream;
import java.io.PrintStream;
import java.io.PrintWriter;
import java.io.IOException;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.URL;
/**
 * Downloads a list of files stored in the server
 */
public class RefreshRequest {
    private static final int BUFFER_SIZE = 2*1048576;
    
    /**
     * Downloads a file from a URL
     * @param fileURL HTTP URL of the file to be downloaded
     * @param saveDir path of the directory to save the file
     * @throws IOException
     */
    public static void getFiles(String fileURL, String saveDir) throws IOException {
        URL url = new URL(fileURL);
        HttpURLConnection httpConn = (HttpURLConnection) url.openConnection();
        int responseCode = httpConn.getResponseCode();
        // always check HTTP response code first
        if (responseCode == HttpURLConnection.HTTP_OK) {
            String fileName = "";
            String disposition = httpConn.getHeaderField("Content-Disposition");
            String contentType = httpConn.getContentType();
            int contentLength = httpConn.getContentLength();
            
            if (disposition != null) {
                // extracts file name from header field
                int index = disposition.indexOf('=');
                if (index > 0) {
                    fileName = disposition.substring
                                (index + 1, disposition.length());
                }
            } else {
                // extracts file name from URL
                fileName = fileURL.substring(fileURL.lastIndexOf("/") + 1,
                       fileURL.length());
            }
            
            // opens input stream from the HTTP connection
            InputStream inputStream = httpConn.getInputStream();
            
            // opens an output stream to save into file and wraps a writer around it
            String saveFilePath = saveDir + File.separator + fileName.substring(1,fileName.length());
            FileOutputStream outputStream = new FileOutputStream(saveFilePath);
            PrintWriter p = new PrintWriter(outputStream);
            
            int bytesRead = -1;
            byte[] buffer = new byte[BUFFER_SIZE];
            while ((bytesRead = inputStream.read(buffer)) != -1) {
                
                // Read the individual bytes and print them to the file if
                // available
                char letter = (char) bytesRead;
                if (bytesRead == 255){
                    continue;
                }
                else if (bytesRead == 10 && (inputStream.read(buffer) == 10)) {
                    p.println();
                }
                else{
                    p.print(letter);
                }
                //outputStream.write(buffer, 0, bytesRead);
            }
            
            // Close stream
            p.close();
            outputStream.close();
            inputStream.close();
        } else {
            System.out.println("No files in the server");
        }

        // Close the http connection
        httpConn.disconnect();
    }
}

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package arduinostore;
 
import java.io.IOException;

/**
 *
 * @author Jerry
 */
public class HttpDownloader {
 
    public static void main(String[] args) {
        String fileURL = "http://www.math.uwaterloo.ca/~snew/Contests/Smallc/Sc2017.pdf";
        String saveDir = "C:/Users/Jerry/Downloads";
        try {
            HttpDownloadUtility.downloadFile(fileURL, saveDir);
        } catch (IOException ex) {
            ex.printStackTrace();
        }
    }
}
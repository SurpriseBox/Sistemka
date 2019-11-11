/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package javacode;

import java.awt.image.*;
import javax.imageio.*;
import java.io.*;
import java.awt.*;
import java.util.Scanner;

class javacode{
    public native void Work(int image[]);
    static{
        System.loadLibrary("abc"); 
    }
    public static void Image_load()
    {
        BufferedImage img;
        Scanner scanner = new Scanner(System.in);
        javacode file_img = new javacode();
        System.out.println("Enter path to image:");
        String path = scanner.nextLine();
        File file = new File(path);
            try{
                img = ImageIO.read(file);
                int pixelbufer[] = new int[img.getWidth()*img.getHeight()];
                for(int y=0; y<img.getHeight();y++){
                    for(int x=0;x<img.getWidth();x++){
                        pixelbufer[y*img.getWidth()+x]=img.getRGB(x,y);
                    }
                }
                file_img.Work(pixelbufer);
                for(int y=0; y<img.getHeight();y++){
                    for(int x=0;x<img.getWidth();x++){
                        img.setRGB(x,y,pixelbufer[y*img.getWidth()+x]);
                    }
                }
                System.out.println("Write to file");
                ImageIO.write(img,"bmp",new File("result.bmp"));
            }
            catch(IOException a){
                System.out.println(a);
            }
        
    }
    public static void main(String[] args) {
        Image_load();
    } 

}

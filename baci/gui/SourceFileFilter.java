package baci.gui;

import java.io.File;
import javax.swing.filechooser.*;
/**
 * FileFilter for source files
 * @author: Moti Ben-Ari
 * @version 20 December 2002
 * See copyright.txt.
 */
public class SourceFileFilter extends FileFilter {

public SourceFileFilter() {
	super();
}
/**
 * Called to see if file is ok
 * @param f
 */
public boolean accept(File f) {
   String ext = f.getName().toUpperCase();
   boolean extOK = false;
   if (f.isFile())
	   extOK = ext.endsWith(".PM") || ext.endsWith(".CM")
    		|| ext.endsWith(".PAS") || ext.endsWith(".C");
   return f.isDirectory() || extOK; 
}
/**
 * Gets the description of this filter
 * @return String
 */
public String getDescription() {
	return "Source Files";
}
}

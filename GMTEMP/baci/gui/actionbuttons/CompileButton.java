package baci.gui.actionbuttons;

import baci.gui.*;
import baci.interpreter.*;
import java.awt.*;
import java.awt.event.*;
import java.io.*;
import javax.swing.*;

/**
 * Action button to compile source file
 * @author: Moti Ben-Ari
 * @version December 2002
 * See copyright.txt
 */
public class CompileButton extends BaciActionButton {

public CompileButton() {
	super(Config.Compile, Config.CompileMN, Config.CompileAC);
}

/**
 * Call compiler depending on source file extension.
 * Errors are displayed in popup frame.
 * Note possible problem with cmd in Windows NT (execute from jar file instead).
 * The compilers should return a completion code that could be 
 * used to enable interpretation or not.
*/
public void actionPerformed(ActionEvent arg1) {
   Editor editor = getDebugger().getDebuggerFrame().getEditor();
   editor.saveFile(null);
   File file = editor.file;
   if (file == null) return;
    String fileName = editor.fileName;
    String absFileName = editor.root + File.separator + fileName;
    String compiler = editor.sourceLanguage.equals("PASCAL") ? 
	Config.getStringProperty("PASCAL_COMPILER") : Config.getStringProperty("C_COMPILER");

   String errorText = "";
   Process p;
   InputStream istream;
   BufferedReader compileError;
   String display = "";
   int exit;
   try {
     p = Runtime.getRuntime().exec(compiler + " " + absFileName, null, editor.file.getParentFile());
     istream = p.getErrorStream();
     compileError = new BufferedReader(new InputStreamReader(istream));
     String s;
     while (true) {
       s = compileError.readLine();
       if (s == null) break; 
       else {
         System.out.println(s);
         errorText = errorText + s + "\n";
       }
     }
     JOptionPane.showMessageDialog(getDebugger().getDebuggerFrame(), (Object) errorText, 
     	"Compilation results", JOptionPane.PLAIN_MESSAGE);
     p.waitFor();
     exit = p.exitValue();
     System.out.println("Compilation completed: " + exit);
     int errorLinePos1 = errorText.indexOf("line ");
     if (errorLinePos1 != -1) {
	     int line = 0;
	     try {
		 line = Integer.parseInt(errorText.substring(errorLinePos1+5, errorText.indexOf("of", errorLinePos1)-1)) - 1;
	     }
	     catch (NumberFormatException e) {}
	     editor.focus( errorLinePos1 == -1 ? 0 : line );
     }
   }
   catch (IOException e) {
     System.out.println("I/O error from: " + compiler + " " + fileName + "  " + e);
   }
   catch (InterruptedException e) {
   }
   catch (IllegalThreadStateException e) {
   }

}
}

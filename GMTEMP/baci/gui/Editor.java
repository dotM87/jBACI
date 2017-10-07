/* Copyright 2003 by Mordechai (Moti) Ben-Ari. See copyright.txt. */
package baci.gui;
import javax.swing.*;
import javax.swing.event.*;
import javax.swing.border.*;
import java.awt.*;
import java.awt.event.*;
import java.awt.datatransfer.*;
import java.io.*;

public class Editor implements ClipboardOwner, DocumentListener {
    public File file;		// The file to be edited
    public String fileName;	// The name of the file
    public String fileRoot;	// The file name without its extension
    public String root;		// The path to the file
    public String sourceLanguage;  // Source language - Pascal or C

    private JTextArea area = new JTextArea();  // Text area for editing
    private String findString;		// The string to search for
    private int findLoc;		// The last location where it was found
    private boolean modified;		// Was area modified?
    private Border border1;		// Border - not modified
    private Border border2;		// Border - modified
    private static final Border border = BorderFactory.createMatteBorder(2,0,0,0,Color.gray);
    private LineNumbers lineNumbers;
    private Clipboard clipboard= area.getToolkit().getSystemClipboard();

    public Editor(JScrollPane jsp) {
        area.setFont(new Font(Config.SOURCE_FONT_FAMILY, 
		Config.SOURCE_FONT_STYLE, Config.getIntegerProperty("SOURCE_FONT_SIZE")));
	root = Config.getStringProperty("SOURCE_DIRECTORY");
	area.getDocument().addDocumentListener(this);
	lineNumbers = new LineNumbers(area);
	jsp.setViewportView(area);
        jsp.setRowHeaderView(lineNumbers);
    }   

    // Request focus for editor text
    public void focus(int line) {
        area.requestFocusInWindow();
	if (line != -1)
	try {
		area.setCaretPosition(area.getLineStartOffset(line));
	}
	catch (Exception e) {
		area.setCaretPosition(0);
	}
    }

    public void caretToLine(int line) {
    	try {
		area.requestFocusInWindow();
		area.setCaretPosition(area.getLineStartOffset(line-1));
	}
	catch (Exception e) { area.setCaretPosition(0); }
    }
    
    public void cut() {
            if (area.getSelectionStart() != area.getSelectionEnd()) {
                String s = area.getSelectedText();
                StringSelection contents = new StringSelection(s);
                clipboard.setContents(contents, this);
                area.replaceRange(null, area.getSelectionStart(), area.getSelectionEnd());
            }
    }

    public void copy() {
            if (area.getSelectionStart() != area.getSelectionEnd()) {
                String s = area.getSelectedText();
                StringSelection contents = new StringSelection(s);
                clipboard.setContents(contents, this);
            }
    }   

    public void paste() {
            Transferable content = clipboard.getContents(this);
            if (content != null) {
                try {
                    String s = (String)content.getTransferData(DataFlavor.stringFlavor);
                    area.replaceRange(s, area.getSelectionStart(), area.getSelectionEnd());
                }
		catch (Exception ex) { System.err.println("Can't paste"); }
            }
    }

    private void showArea(String title) {
	    findString = null; findLoc = 0;
            modified = false;
	    border1 = BorderFactory.createTitledBorder(border, title, TitledBorder.LEFT, TitledBorder.TOP);
	    border2 = BorderFactory.createTitledBorder(border,  title + " * ", TitledBorder.LEFT, TitledBorder.TOP);
	    area.setBorder(border1);
	    focus(0);
    }

    public void newFile() {
	    root = ""; file = null; fileName = ""; fileRoot = "";
	    area.setText("");
	    showArea(" ");
    }
    
    private void setRootAndName() {
	    fileName = file.getName();
	    root = file.getParentFile().getAbsolutePath();
	    fileRoot = fileName.substring(0, fileName.lastIndexOf('.'));
	    sourceLanguage = (fileName.toUpperCase().endsWith(".PM") 
		    || fileName.toUpperCase().endsWith(".PAS")) ? "PASCAL" : "C";
	    showArea(fileName);
    }

    public void openFile(File fc) {
	if (!fc.exists()) {
            JOptionPane.showMessageDialog(area, "File " + fc + " does not exist", "", JOptionPane.PLAIN_MESSAGE);
	    return;
	}
	file = fc;
	area.setText(readFile(fc));
	setRootAndName();
    }
	
    public String readFile(File fc) {
        BufferedReader textReader = null;
        try {
            textReader = new BufferedReader(new FileReader(fc));
        }
        catch (IOException e) {
            return "Error opening file " + fc; 
        }
        StringWriter textWriter = new StringWriter();
        int c = 0;       
        try {   
            while (true) {
                c = textReader.read();
                if (c == -1) break; 
		else textWriter.write(c);   
            }
        }       
        catch (IOException e) {
            return "Error reading file " + fc;
        }
        return textWriter.toString();
    }

    public void saveFile(File fc) {
	if (fc != null) {
		file = fc;
		setRootAndName();
		modified = true;
	}
	if (modified) {
		area.setBorder(border1);
		writeFile(file, area);
		modified = false;
	}
    }

    public void writeFile(File fc, javax.swing.text.JTextComponent area) {
	if (fc == null || area == null) return;
        BufferedWriter textWriter = null;
        try {
            textWriter = new BufferedWriter(new FileWriter(fc));
        }
        catch (IOException e) {
            System.err.println("Error opening file " + fc); 
            return;
        }
        BufferedReader textReader = new BufferedReader(new StringReader(area.getText()));
        int c = 0;
        try {   
            while (true) {
                c = textReader.read();
                if (c == -1) break; 
		else textWriter.write(c);
            }
        }       
        catch (IOException e) {
            System.err.println("Error writing file " + fc);
            return;
        }   
    
        try {
            textWriter.flush();
            textWriter.close();
        }               
        catch (IOException e) {
            System.err.println("Error closing file " + fc);
            return;
        }   
    }

    public void find() {
	findString = JOptionPane.showInputDialog(area, null, "Find", JOptionPane.PLAIN_MESSAGE);
	if (findString != null) {
		search();
		findLoc = area.getCaretPosition();
	}
	else focus(-1);
    }

    public void findAgain() {
	if (findString != null) search();
	else focus(-1);
    }

    private void search() {
            int found = area.getText().toLowerCase().indexOf(findString.toLowerCase(), findLoc);
            if (found != -1) {
		findLoc = found + 1;
                area.setCaretPosition(found);
                area.moveCaretPosition(found+findString.length());
            }
            focus(-1);
        }

    private void setModified(DocumentEvent e) {
	    modified = true;
	    area.setBorder(border2);
	    final int lines = area.getLineCount();
	    if (lineNumbers != null) {
		    Runnable updateAComponent = new Runnable() {
			    public void run() {
				    lineNumbers.setHeightByLines(lines);
			    }
		    };
		    SwingUtilities.invokeLater(updateAComponent);
	    }
    }
    public void changedUpdate(DocumentEvent e) { setModified(e); } 
    public void insertUpdate(DocumentEvent e)  { setModified(e); }
    public void removeUpdate(DocumentEvent e)  { setModified(e); } 

    public void lostOwnership(Clipboard clipboard, Transferable content) {}
}

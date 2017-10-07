package baci.interpreter;

import javax.swing.*;
import javax.swing.text.*;
/**
 * Stores data written to output by a process
 * @author: David Strite
 */
public class Console extends PlainDocument {
/**
 * Creates a new Console
 */
public Console() {
	super();
}
/**
 * Clears all text from this console
 */
public void clear() {
	try {
		remove(0, getLength());
	} catch (BadLocationException ble) {
		java.awt.Toolkit.getDefaultToolkit().beep();
		ble.printStackTrace();
	}
}
/**
 * Adds the specified String to this console
 * @param s string to print
 */
public void print(String s) {
	try {
		insertString(getLength(),s,null);
	} catch (BadLocationException ble) {
		java.awt.Toolkit.getDefaultToolkit().beep();
		ble.printStackTrace();
	}
}
}

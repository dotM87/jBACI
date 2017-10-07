package baci.gui;

import java.awt.*;
import javax.swing.*;
import javax.swing.table.*;
/**
 * Table cell render for code status cell
 * @author: David Strite
 */
public class StatusCellRenderer extends JLabel implements TableCellRenderer {
	private Object tempValue;
/**
 * Constructs new SourceStatusCellRenderer
 */
public StatusCellRenderer() {
	setBorder(null);
	setOpaque(true);
}
/**
 * Called by table to get cell renderer component
 * @param table
 * @param value
 * @param isSelected
 * @param hasFocus
 * @param row
 * @param column
 */
public java.awt.Component getTableCellRendererComponent(JTable table, Object value, boolean isSelected, boolean hasFocus, int row, int column) {
	if (isSelected) {
		setForeground(table.getSelectionForeground());
		setBackground(table.getSelectionBackground());
	} else {
		setForeground(table.getForeground());
		setBackground(table.getBackground());
	}
	setFont(table.getFont());
	tempValue = value;
	return this;
}
/**
 * Called to paint component
 * @param g
 */
public void paint(Graphics g) {
	super.paint(g);
	if(tempValue.equals("S")) {
		//Current and breakpoint
		g.setColor(Color.red);
		g.fillPolygon(new int[]{2,2,14},new int[]{2,14,8},3);
	} if(tempValue.equals("B")) {
		//Breakpoint
		g.setColor(Color.red);
		g.fillOval(2,2,12,12);
	} else if (tempValue.equals("C")) {
		//Current
		g.setColor(Color.green);
		g.fillPolygon(new int[]{2,2,14},new int[]{2,14,8},3);
	}
}
}

package baci.gui.actionbuttons;

import baci.gui.*;
import java.awt.event.*;
import java.awt.Font;
import javax.swing.*;

/**
 * Action button to display About
 * @author: Moti Ben-Ari
 * @version December 2002
 * See copyright.txt
 */
public class AboutButton extends BaciActionButton {
/**
 * Constructs a new AboutButton
 */
public AboutButton() {
    super(Config.About, Config.AboutMN);
    setEnabled(true);
}
/**
 * Display about
 * @param arg1
 */
public void actionPerformed(ActionEvent arg1) {
    String aboutText = 
"              " + Config.SOFTWARE_NAME + "\n\n" +
" Copyright 2002-3 by Mordechai (Moti) Ben-Ari.\n" +
" Modified from BACI Debugger, Copyright 1999 by David Strite.\n\n" +
" This program is free software; you can redistribute it and/or\n" +
" modify it under the terms of the GNU General Public License\n" +
" as published by the Free Software Foundation; either version 2\n" +
" of the License, or (at your option) any later version.\n\n" +
" This program is distributed in the hope that it will be useful\n" +
" but WITHOUT ANY WARRANTY; without even the implied warranty of\n" +
" MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n" +
" See the GNU General Public License for more details.\n\n" +
" You should have received a copy of the GNU General Public License\n" + 
" along with this program; if not, write to the Free Software\n" +
" Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA \n" + 
" 02111-1307, USA."
;  
    JOptionPane.showMessageDialog(getDebugger().getDebuggerFrame(), (Object) aboutText,
         "jBACI", JOptionPane.PLAIN_MESSAGE);
}
}

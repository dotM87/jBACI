# Tcl/Tk code for the GUI version of the BACI Interpreter 
#
#  based on original version by Vince Gibson, University of Alabama
#           


global CurrentPCOFile   ;# name of the current .pco file
global InterpPName      ;# interpreter name
global debugging        ;# 1 when a breakpoint is set
global ShowPCODE        ;# 1 when showing PCODE
global ShowProcWins     ;# 1 when showing process windows

global currSrcWin       ;# array to hold filenames of the source windows

global cancelledChoice     ;# was a choice cancelled? 1 if so, 0 if not

set ShowProcWins 1      ;# default is showing process windows
set cancelledChoice 0

global LastSrcHL           ;# holds last source line no highlighted
set LastSrcHL ""

option add *Font "-misc-fixed-medium-r-normal-*-14-*-*-*-*-*-*-*"

set dir [pwd]
set fonttype -misc-fixed-medium-r-normal-*-14-*-*-*-*-*-*-*

####  initWindowPosition
#
#  sets the initial position of the current window
#
proc initWindowPosition {} {
   global currSrcWin
   set currSrcWin(X) 0
   set currSrcWin(Y) 90
}  ;# initWindowPosition

####  OKdialog
#
#  An OK modal dialog to display a message to the user
#
proc OKdialog {wtitle dlg_text} {
   if [winfo exists .okdlg] {return ""}  ;# double-click insurance
   toplevel .okdlg
   label .okdlg.l -text "$dlg_text"
   button .okdlg.ok -text OK -command { destroy .okdlg }
   wm geometry .okdlg +400+400
   wm title .okdlg $wtitle
   pack .okdlg.l -side top
   pack .okdlg.ok -side top -fill x -expand 1
   set win [grab current]
   tkwait visibility .okdlg
   grab set .okdlg
   tkwait window .okdlg
   if { $win != "" } { grab set $win }
}  ;# OKdialog

######  Message
#
#  creates a temporary (5 sec) window containing $msg
#  Message windows are staggered so multiple messages can be seen
#     simultaneously
#
proc Message { msg } {
   global msgcnt
   global mypos
   if {! [info exists msgcnt] } {
      set msgcnt 0
      set mypos($msgcnt) 30
   }
   if [winfo exists .msg$msgcnt] {
      set g [split [wm geometry .msg$msgcnt] +]
      set dely [lindex [split [lindex $g 0] x] 1]
      set tmp $msgcnt
      incr msgcnt
      set mypos($msgcnt) [expr $mypos($tmp) + $dely + 30]
   } else {
      set mypos($msgcnt) 30
   }

   toplevel .msg$msgcnt
   wm title .msg$msgcnt "BACI Note"
   wm geometry .msg$msgcnt +30+$mypos($msgcnt)
   label .msg$msgcnt.l -text "$msg"
   pack .msg$msgcnt.l -side top -padx 10 -pady 10
   after 5000 [list destroy .msg$msgcnt]
   tkwait visibility .msg$msgcnt
}  ;# Message

####  aboutgui
#
#  A window to explain about the BACI gui
#
proc aboutgui { } {
   OKdialog "BACI GUI" \
"
The Graphical User Interface of the
BenAri Concurrent PCODE Interpreter

was developed by

Bill Bynum and Tracy Camp

from the original design by

Vince Gibson
"
}  ;# aboutgui

####  destroyWindow
#
#  Used whenever a window needs to be destroyed
#  Caller is expected to check for the existence of the window
#
proc destroyWindow { w } {
   if [catch [list destroy $w] tmp] {
      puts stderr "Couldn't destroy $w\n$tmp"
   }
}  ;# destroyWindow
   
####  makemainwin
#
#  Make the window for the main process
#
proc makemainwin {} {
   global wr_proc_index
   global .process0
   if [winfo exists .process0] {
      onewinprint "Restarting ************" 0
      return
   }
   toplevel .process0 -class Dialog
   wm title .process0 Main
   wm geometry .process0 500x300+500+430
   text .process0.text -relief sunken -bd 2 \
      -yscrollcommand ".process0.scroll set"
   scrollbar .process0.scroll -command ".process0.text yview"
   pack .process0.scroll -side right -fill y
   pack .process0.text -fill both -expand yes
   pack .process0.text -side right -expand 1 -fill both -padx 3m -pady 3m
   .process0 configure -background green
   .mbar.fb.menu entryconfigure $wr_proc_index -state active
}  ;# makemainwin

####  makePCOwin
#
#  Make the PCODE window
#  Change the state of the "Write PCODE window" entry in the File menu
#  if necessary
#
proc makePCOwin { } {
   global wr_pco_index
   set w .process411
   global CurrentPCOFile
   global ShowPCODE
   if { ! $ShowPCODE } {   ;# showing PCODE has been disabled
      .mbar.fb.menu entryconfigure $wr_pco_index -state disabled
      if [winfo exists $w] {
         destroy $w
      }
      return
   }
   if { ! [winfo exists $w] } { 
      set ShowPCODE 1
      .mbar.fb.menu entryconfigure $wr_pco_index -state active
      toplevel $w -class Dialog
      wm title $w "BACI PCODE"

      text $w.text -yscrollcommand [list $w.scroll set]
      scrollbar $w.scroll -command [list $w.text yview]
      pack $w.scroll -side right -fill y
      pack $w.text -fill both -expand 1
      pack $w.text -side right -expand 1 -fill both 
      wm geometry $w 600x300
   }
   $w.text config -state normal
   $w.text insert end "$CurrentPCOFile execution     [exec date]\n"
   $w.text insert end "proc  lc    f    x    y     disassembled instruction\n"
   $w.text configure -state disabled
}  ;# makePCOwin

####  toggleProcWins
#
#  take the appropriate action when user toggles "show process windows" 
#
proc toggleProcWins { } {
   global spwin_index
   global wr_proc_index
   global ShowProcWins

   if {$ShowProcWins} {
      .mbar.fb.menu entryconfigure $wr_proc_index -state active
   } else {
      for {set i 1} {[winfo exists .process$i]} {incr i} {
         destroyWindow .process$i
      }
   }
}

####  getstr
#
#  read string typed by user in a given window
#  
proc getstr { pno } {
   set w .process$pno
   $w.text configure -state normal
   grab set $w
   focus $w.text
   set iplace [$w.text index insert]
   bind $w.text <Return> {set eol 1}
   tkwait variable eol
   set tmp [$w.text get $iplace "$iplace lineend"]
   $w.text config -state disabled
   grab release $w
   return $tmp
}  ;# getstr

####  makeDATAwin
#

####  makeDATAwin
#
#  Make the window in which to display data
#  Change status of "Write Data window" in File menu if necessary
#
proc makeDATAwin { } {
   global wr_data_index
   global CurrentPCOFile
   set w .process505
   global $w
      
   if [winfo exists $w ] { 
      return 
   } else {
      .mbar.fb.menu entryconfigure $wr_data_index -state disabled
   }
   toplevel $w -class Dialog
   wm title $w "BACI Data"
   .mbar.fb.menu entryconfigure $wr_data_index -state active
   text $w.text -yscrollcommand [list $w.scroll set]
   scrollbar $w.scroll -command [list $w.text yview]
   pack $w.scroll -side right -fill y
   pack $w.text -fill both -expand 1
   pack $w.text -side right -expand 1 -fill both 
   wm geometry $w 600x300
   $w.text configure -state normal
   $w.text insert end "$CurrentPCOFile execution data [exec date]\n"
   $w.text configure -state disabled
}  ;# makeDATAwin

####  overwrite
#
#  Create an Overwrite? dialog for the file $fname, if $fname exists
#
proc overwrite { fname } {
   set ans 1
   if [file exists $fname] {
      global .ow
      toplevel .ow
      wm title .ow "Overwrite???"
      wm geometry .ow +400+400
      label .ow.l -text "File \"$fname\" exists!"
      frame .ow.f
      button .ow.f.ok -text Overwrite -relief raised -bd 2 \
         -command { set ans 1 }
      button .ow.f.cancel -text Cancel -relief raised -bd 2 \
         -command { set ans 0 }
      pack .ow.f.ok .ow.f.cancel -side left -in .ow.f -fill both -expand 1
      pack .ow.l .ow.f -side top -expand 1 -fill both
      set win [grab current ]
      grab set .ow 
      tkwait variable ans
      destroy .ow
      if { $win != "" } { grab set $win }
      return $ans
   }
   return $ans
}  ;# overwrite

####  write_file
#
#  write the contents of the text window $tw.text to the 
#  file named $fname
#  destroy window $w afterwards
#
proc write_file { tw fname w} {
   if { [ overwrite $fname ] } {
      set fid [open $fname "w"]
      puts $fid [ $tw.text get  1.0 end ]
      close $fid 
   }
   destroy $w
}  ;# write_file

####  writeAwin
#
#  A modal dialog to see where the user wants write the
#  the PCODE or DATA window ($wtype).  
#  To simplify display to different windows, all BACI window
#  names have the name .process$pno.  $pno here is the
#  proper number for the PCODE or DATA "process" window.
#  The name of the created file is composed of the 
#  concatenation of the root name of the .pco file 
#  and the suffix $fsfx.
#
proc writeAwin {pno wtype fsfx } {
   global .wdlg
   global CurrentPCOFile
   global WFname
   set w .process$pno
   if { ! [winfo exists $w] } {
      return
   }
   toplevel .wdlg -class Dialog
   wm title .wdlg "Write $wtype Window?"
   frame .wdlg.f
   label .wdlg.f_lab -text "Write $wtype window to:" 
   entry .wdlg.f_ent -width 30 -relief sunken -textvariable WFname
   set initWFname [ file rootname $CurrentPCOFile ].$fsfx
   .wdlg.f_ent delete 0 end
   .wdlg.f_ent insert end $initWFname
   pack .wdlg.f_lab .wdlg.f_ent -side left -in .wdlg.f
   frame .wdlg.b -bd 1
   button .wdlg.b.ok -text OK -relief raised -bd 2 \
      -command [list write_file $w $WFname .wdlg]
   button .wdlg.b.cancel -text Cancel -relief raised -bd 2 \
      -command { destroy .wdlg } 
   pack .wdlg.b.ok .wdlg.b.cancel -side left -in .wdlg.b -fill both -expand 1
   pack .wdlg.f .wdlg.b -side top -fill both -expand 1
   tkwait window .wdlg
}  ;# writeAwin


####  writeprocwins
#
#  Write the process windows
#
proc writeprocwins {} {
   global .wdlg2
   global CurrentPCOFile
   global ShowProcWins
   if { ! [winfo exists .process0] } {
      return
   }
   toplevel .wdlg2 -class Dialog
   wm title .wdlg2 "Write Process Windows?"
   frame .wdlg2.f
   label .wdlg2.f_lab -text "Write Process Windows to:" 
   entry .wdlg2.f_ent -width 30 -relief sunken -textvariable PCOFname
   set initPCOFname [ file rootname $CurrentPCOFile ].prw
   .wdlg2.f_ent delete 0 end
   .wdlg2.f_ent insert end $initPCOFname
   pack .wdlg2.f_lab .wdlg2.f_ent -side left -in .wdlg2.f
   frame .wdlg2.b -bd 1
   button .wdlg2.b.ok -text OK -relief raised -bd 2 \
      -command { if { [ overwrite $PCOFname ] } {
               set fid [open $PCOFname "w"]
               puts $fid "$CurrentPCOFile execution     [exec date]"
               for {set i 0} {[winfo exists .process$i]} {incr i} {
                  puts $fid "Process $i Window"
                  puts $fid [ .process$i.text get 1.0 end ]
               }
               close $fid 
            }
            destroy .wdlg2
         } 
   button .wdlg2.b.cancel -text Cancel -relief raised -bd 2 \
      -command { destroy .wdlg2 }
   pack .wdlg2.b.ok .wdlg2.b.cancel -side left -in .wdlg2.b -fill both \
      -expand 1
   pack .wdlg2.f .wdlg2.b -side top -fill both -expand 1
   tkwait window .wdlg2
}  ;# writeprocwins

####  nyi
#
#  Not Yet Implemented modal dialog
#
proc nyi { } {
   OKdialog "BACI" "Not yet implemented"
}  ;# nyi

####  set_pc_break
#
#  Set a breakpoint at $addr 
#  Mark the appropriate line number of the current file
#  with a 'b'
#  Clear the breakpoint entry if the breakpoint is
#  sucessfully set
#
proc set_pc_break { addr } {
   global tlast_code
   if { "$addr" == "" } {
      Message "Enter a PCODE address
or go to another entry"
      return
   }
   if { $addr > $tlast_code } {
      Message "Address $addr is larger than $tlast_code,
the largest PCODE address"
      .cb.pf.pe delete 0 end
      return
   } 
   set u [setBreakpoint $addr ]
   if { [lindex $u 1 ] >= 0 } {
      .cb.pf.pe delete 0 end
      mark_breakpoint $u
      rebuild_bplist
   }
}  ;# set_pc_break

####  make_pc_pane
#
#  Create a pane of the breakpoint window to get a pc value 
#  for setting a breakpoint
#  
proc make_pc_pane { } {
   global bpco_addr
   global debugging
   set debugging 1
   frame .cb.pf
   label .cb.pf.pl -text "Enter PCODE address for a breakpoint" -justify left
   entry .cb.pf.pe -relief sunken -width 6 -textvariable bpco_addr
   set bpco_addr ""
   bind .cb.pf.pe <Return> {set_pc_break $bpco_addr}
   pack .cb.pf.pl -in .cb.pf -side left -padx 1 -pady 1 -anchor w
   pack .cb.pf.pe -in .cb.pf -side left -padx 1 -pady 1
   pack .cb.pf -side top -fill both -expand true
   button .cb.pb1 -text "Set breakpoint" -command {set_pc_break $bpco_addr}
   pack .cb.pb1 -side top -expand true -fill both
   frame .cb.pr1 -width 30 -bg black
   pack .cb.pr1 -side top -fill x -expand 1 -pady 3
}  ;# make_pc_pane
      
####  rebuild_bplist
#
#  Rebuild the breakpoint list in the .cb.f.lb listbox
#
proc rebuild_bplist { } {
   global tbreak_cnt
   .cb.f.lb delete 0 end
   for {set ix 0} { $ix < $tbreak_cnt} {incr ix} {
      set u [getBreakpoint $ix]
      .cb.f.lb insert end $u
   }
}  ;# rebuild_bplist

####  delete_break
#
#  Delete the breakpoint corresponding to the currently selected 
#  line of the .cb.f.lb listbox.  
#  Scold the user if no line of the listbox is selected; 
#  otherwise, delete the breakpoint and blank the first char
#  of the source line corresponding to the break
#
proc delete_break { } {
   set cs [.cb.f.lb curselection]
   if { $cs == "" } { 
      Message "Select a breakpoint\nto delete!"
      return
   } 
   .cb.f.lb delete $cs
   if [catch [list unsetBreakpoint $cs] u] {
      Message $u
      return
   }
   set fno [lindex $u 0]
   set lno [lindex $u 1]
   set tmp [LineHasBreak $fno $lno]
   if {! $tmp } {
      set uw .source$fno
      $uw.text configure -state normal
      $uw.text delete $lno.0 $lno.1
      $uw.text insert $lno.0 " "
      $uw.text configure -state disabled
   }
   rebuild_bplist
}  ;# delete_break

####  mark_breakpoint
#
#  Change the first char of the breakpointed line number [lindex $u 1]
#  of the source window .source$[lindex $u 0] to a 'b', if the window
#  exists
#  
proc mark_breakpoint { u } {
   set lno [lindex $u 1]
   set fno [lindex $u 0]
   if { $lno < 0 } { return }
   set uw .source$fno
   if [winfo exists $uw] {
      $uw.text configure -state normal
      $uw.text delete $lno.0 $lno.1
      $uw.text insert $lno.0 "b"
      $uw.text configure -state disabled
   }  ;# if the window exists
}  ;# mark_breakpoint
      
####  set_srcln_break
#
#  Set a break at source line $bsl_addr of file with 
#  index $curr_file.  
#  Change the ' ' in front of the line no. with a 'b'.
#
proc set_srcln_break { current_file bsl_addr } {
   if {$bsl_addr == "" } {
      Message "Enter a line #\nor go to another entry"
   } else {
      set pc  [LinenoToPC $current_file $bsl_addr ]
      set u [setBreakpoint $pc]
      if {[lindex $u 0] != $current_file} {
         puts stderr "File index problem in set_srcln_break: wanted \
$current_file got $u"
         #exit 
      }
      if {$pc != -1} {
         mark_breakpoint $u
         .cb.sf0.se delete 0 end
         rebuild_bplist
      }
   }
}  ;# set_srcln_break

####  set_curr_file
#
#  Set the name on the menu button .cs.sf.mb 
#  to the file $infname($fix).  
#  Set the curr_file global to $fix.
#  Only called when the is more than one input file.
#
proc set_curr_file { fix } {
   global curr_file
   global infname
   .cb.sf.mb configure -text $infname($fix)
   if [winfo exists .source$fix] {
      raise .source$fix
   }
   set curr_file $fix
}  ;# set_curr_file

####  make_srcln_pane
#
#  Create the pane of the breakpoint window with which a user 
#  can choose a source line for a breakpoint.
#
proc make_srcln_pane {} {
   global bsl_addr
   global infname
   global curr_file
   global tlast_infile
   global debugging
   set debugging 1

   frame .cb.sf
   label .cb.sf.l1 -text "Source file:" -width 13
   pack .cb.sf.l1 -in .cb.sf -side left -padx 1 -pady 1 -fill both -expand 1
   if {$tlast_infile == 0} {
      label .cb.sf.l3 -width 20  -text $infname($curr_file)
      pack .cb.sf.l3 -in .cb.sf -side left -padx 1 -pady 1 -fill both \
         -expand 1
   } else {
      menubutton .cb.sf.mb -text $infname($curr_file) -menu .cb.sf.mb.m \
         -borderwidth 3 -relief raised -width 20 
      menu .cb.sf.mb.m -tearoff 0
      for {set j 0} {$j <= $tlast_infile} { incr j } {
         .cb.sf.mb.m add radio -label $infname($j) -variable curr_file \
           -value $j  -command [list set_curr_file $j]
      }
      pack .cb.sf.mb -in .cb.sf -side left -expand 1 -fill both
   }
   pack .cb.sf -side top -fill both -expand 1
   frame .cb.sf0 
   label .cb.sf0.l -text "Enter source line number for a breakpoint" \
      -justify left
   pack .cb.sf0.l -in .cb.sf0 -side left -padx 1 -pady 1 -anchor w
   entry .cb.sf0.se -relief sunken -width 6 -textvariable bsl_addr
   pack .cb.sf0.se -in .cb.sf0 -side left -padx 1 -pady 1 
   bind .cb.sf0.se <Return> { set_srcln_break $curr_file $bsl_addr}
   pack .cb.sf0 -side top -fill both -expand true 
   set bsl_addr ""
   button .cb.sb1 -text "Set breakpoint" \
      -command {set_srcln_break $curr_file $bsl_addr}
   pack .cb.sb1 -side top -expand true -fill both
   frame .cb.sr1 -width 30 -bg black
   pack .cb.sr1 -side top -fill x -expand 1 -pady 3
}  ;# make_srcln_pane


####  set_cb_bindings
#
#  Set bindings for the entries and buttons of the .cb window
#
proc set_cb_bindings { } {
   global tlast_infile
   bind .cb.pf.pe <Return> {focus .cb.pb1; break}
   bind .cb.pf.pe <Tab> {focus .cb.pb1; break}
   if {$tlast_infile != 0 } {
      bind .cb.pb1 <Return> {focus .cb.sf.mb; break}
      bind .cb.pb1 <Tab> {focus .cb.sf.mb; break}
      bind .cb.sf.mb <Return> {focus .cb.se; break}
      bind .cb.sf.mb <Tab> {focus .cb.sf0.se; break}
   } else {
      bind .cb.pb1 <Return> {focus .cb.sf0.se; break}
      bind .cb.pb1 <Tab> {focus .cb.sf0.se; break}
   }   
   bind .cb.sf0.se <Return> {focus .cb.sb1; break}
   bind .cb.sf0.se <Tab> {focus .cb.sb1; break}
   bind .cb.sb1 <Return> {focus .cb.f.lb; break}
   bind .cb.sb1 <Tab> {focus .cb.f.lb; break}
   bind .cb.f.lb <Return> {focus .cb.b2; break}
   bind .cb.f.lb <Tab> {focus .cb.b2; break}
   bind .cb.b2 <Return> {focus .cb.b1; break}
   bind .cb.b2 <Tab> {focus .cb.b1; break}
   bind .cb.b1 <Return> {focus .cb.pf.pe; break}
   bind .cb.b1 <Tab> {focus .cb.pf.pe; break}
}  ;# set_cb_bindings

####  choose_breakpoint
#
#  Create a window that allows the user to choose a breakpoint
#
proc choose_breakpoint { } {
   global curr_file
   if [winfo exists .cb] {raise .cb; return ""}  ;# double-click insurance
   if {(! [info exists curr_file])||($curr_file == "") } {
      Message "Open a .pco file to run"
      return
   }
   toplevel .cb
   wm title .cb "BACI breakpoints"
   
   label .cb.l0 -justify left -text \
"TAB or RETURN ==> go to next entry or button
SPACE ==> click button or select entry"
   pack .cb.l0 -side top -padx 3 -pady 3
   frame .cb.r1 -width 30 -bg black
   pack .cb.r1 -side top -fill x -expand 1 -pady 3
   make_pc_pane
   make_srcln_pane
   
   frame .cb.f
   label .cb.f.l2 -justify left -anchor w \
      -text " ix   loc  block        line  filename"
   listbox .cb.f.lb -yscroll ".cb.f.scr set" -relief sunken \
      -width 40 -height 8
   scrollbar .cb.f.scr -command ".cb.f.lb yview" 
   pack .cb.f.l2 -side top -expand 1 -fill both
   pack .cb.f.lb -side left -fill both -expand 1
   .cb.f.lb delete 0 end
   pack .cb.f.scr -in .cb.f -side right -fill both
   pack .cb.f -side top -fill both -expand 1
   button .cb.b2 -text "Delete selected breakpoint" -command { delete_break }
   pack .cb.b2 -side top -fill both -expand 1
   button .cb.b1 -text "Dismiss" -command { destroy .cb}
   pack .cb.b1 -side top -fill both -expand 1
   rebuild_bplist
   set_cb_bindings
}  ;# choose_breakpoint


####  break_alert
#
#  Called by C break_alert proc to produce a break alert
#  Raise the source window of the current source file, if
#  it exists.
#
proc break_alert { m bpwin} {
   global curr_file
   if [winfo exists .msg] { return }
   toplevel .msg
   if { $bpwin } {
      wm title .msg "BACI Breakpoint"
   } else {
      wm title .msg "BACI Pause"
   }
   wm geometry .msg +580+0
   label .msg.l -text $m
   pack .msg.l -side top -padx 2m -pady 2m
   if [winfo exists .source$curr_file] {
      raise .source$curr_file
   }
}  ;# break_alert

####  remove_alert
#
#  Called by C remove_alert proc to remove the alert
#  msg posted by break_alert
#  Clear the highlighting on the breakpointed line.
#
proc remove_alert { } {
   global LastSrcHL
   if [winfo exists .msg ] {
      destroy .msg
   }
   set w .source$LastSrcHL
   if [winfo exists $w] {
      if { $LastSrcHL != "" } {
         $w.text configure -state normal
         $w.text tag delete highlight
         $w.text configure -state disabled
         set LastSrcHL ""
      }
   }
}  ;# remove_alert

####  disable_openfile
#
#  Disables the "Open a PCODE File" and "Reload PCODE" choices
#  in the File menu
#
proc disable_openfile { } {
   global open_file_index
   global reload_pco_index
   .mbar.fb.menu entryconfigure $open_file_index -state disabled
   .mbar.fb.menu entryconfigure $reload_pco_index -state disabled
}  ;# disable_openfile

####  enable_openfile
#
#  Enables the "Open a PCODE File" and "Reload PCODE" choices
#  in the File menu
#
proc enable_openfile { } {
   global open_file_index
   global reload_pco_index
   .mbar.fb.menu entryconfigure $open_file_index -state active
   .mbar.fb.menu entryconfigure $reload_pco_index -state active
}  ;# enable_openfile

####  setup
#
#  Set up the BACI main menu bar
#
proc setup {} {
   global wr_pco_index 
   global wr_data_index 
   global wr_proc_index 
   global open_file_index 
   global reload_pco_index 
   global spwin_index
   menubutton .mbar.fb -text File -underline 0 -menu .mbar.fb.menu
   menubutton .mbar.ob -text Options -underline 0 -menu .mbar.ob.menu
   menubutton .mbar.db -text Data -underline 0 -menu .mbar.db.menu
   button .mbar.bb -text Breakpoints -relief flat \
      -command { choose_breakpoint }
   button .mbar.ab -text About -underline 0 -relief flat \
      -command {aboutgui}

   pack .mbar.fb .mbar.ob .mbar.db .mbar.bb -side left -padx 3 
   pack .mbar.ab -side right -padx 10

   frame .top -bd 1
   pack .top -side top -fill both
   frame .bot -relief sunken -bd 1
   pack .bot -side bottom -fill both -expand 1

   button .bot.run -text Run -relief raised -bd 2 -state disabled \
      -command { bacirun 1 }
   button .bot.continue -text Continue -relief raised -bd 2 -state disabled \
      -command {set xpause 0; disable_openfile}
   button .bot.1p -text "1 pco" -relief raised -bd 2 -state disabled \
      -command {set xpause 1; disable_openfile}
   button .bot.1s -text "1 source" -relief raised -bd 2 -state disabled \
      -command {set xpause 2; disable_openfile}
   button .bot.pause -text Pause -relief raised -bd 2 -state disabled \
      -command {set xpause -1}
   button .bot.stop -text Stop -relief raised -bd 2 -state disabled \
      -command {set xpause 999; enable_openfile; disablerunbuttons; \
         remove_alert}
   button .bot.exit -text Exit -relief raised -bd 2 -state active \
      -command {shutdown}
   pack .bot.run .bot.continue .bot.1p .bot.1s .bot.pause \
      .bot.stop .bot.exit -side left -padx 1m -expand 1 -fill both

   set f [menu .mbar.fb.menu -tearoff 0]
   set o [menu .mbar.ob.menu -tearoff 0]
   set d [menu .mbar.db.menu -tearoff 0]

   bind . <Control-f> {.mbar.fb.menu post 10 10}

   $f add command -label "Open a PCODE file" -underline 0 -command {openFile}
   set open_file_index 0
   $f add command -label "Reload current PCODE file" -underline 0 \
      -command {reloadPCODE}
   set reload_pco_index 1
   $f add separator
   $f add command -label "Write PCODE Window" -underline 6 \
      -command {writeAwin 411 PCODE pcw} -state disabled
   set wr_pco_index 3
   $f add command -label "Write DATA Window" -underline 6 \
      -command {writeAwin 505 DATA daw} -state disabled
   set wr_data_index 4
   $f add command -label "Write Process Windows" -underline 7 \
      -command {writeprocwins}
   set wr_proc_index 5
   $f entryconfigure $reload_pco_index -state disabled
   $f entryconfigure $wr_pco_index -state disabled
   $f entryconfigure $wr_data_index -state disabled
   $f entryconfigure $wr_proc_index -state disabled


   $o add check -label "Show AR on entry" -underline 12 -variable tinitAR_db \
      -command {makeDATAwin}
   $o add check -label "Show AR on exit" -underline 12 -variable tfinalAR_db \
      -command {makeDATAwin}
   $o add check -label "Announce process termination" -underline 0 \
      -variable tend_db -command {makeDATAwin}
   $o add separator
   $o add check -label "Debug PCODE" -underline 0 -variable ShowPCODE \
      -command { makePCOwin }
   set spwin_index 5
   $o add check -label "Show process windows"  -variable ShowProcWins\
      -command {toggleProcWins} 

   $d add command -label "Global Variables" \
      -command {makeDATAwin; WriteData global_vars}
   $d add command -label "Monitor Variables" \
      -command {makeDATAwin; WriteData monitor_vars}
   $d add command -label "Process Variables" \
      -command {makeDATAwin; WriteData process_vars}
   $d add command -label "Variables of the Current Function" \
      -command {makeDATAwin; WriteData currproc_vars}
   $d add command -label "Process Table" \
      -command {makeDATAwin; WriteData process_table}
   $d add command -label "Runtime Stacks" \
      -command {makeDATAwin; WriteData stacks}

}  ;# setup

####  shutdown
#  
#  Allow all parts of the interpreter to complete, 
#  then exit
#
proc shutdown { } {
   global quitting
   global xpause
   set xpause 999
   update idletasks
   after 500 
   set quitting 1
}  ;# shutwdown

####  closeSrcWindows
#
#  Close all of the source windows that currently exist
#
proc closeSrcWindows {} {
   for {set i 0} {[winfo exists .source$i]} {incr i} {
      destroyWindow .source$i
   }
}  ;# closeSrcWindows

####  closeProcWindows
#
#  Close all ofthe process windows that exist
#
proc closeProcWindows {} {
   for {set i 0} {[winfo exists .process$i]} {incr i} {
      destroyWindow .process$i
   }
}  ;# closeProcWindows

#### endcolorProcWindows
#
#  Set the color of the window boundary of each 
#  real (i.e., not PCODE or DATA) process window that exists
#
proc endcolorProcWindows {} {
   for {set i 0} {[winfo exists .process$i]} {incr i} {
      .process$i configure -background red
   }
}  ;# endcolorProcWindows
   
####  closeWindows
#
#  Close all windows except .
#
proc closeWindows {} {
   global wr_proc_index
   global wr_data_index
   global wr_pco_index
   closeSrcWindows
   closeProcWindows
   if [winfo exists .cb ] {
      destroyWindow .cb
   }
   if [winfo exists .msg ] {
      destroyWindow .msg
   }
   if [winfo exists .process505] {
      destroyWindow .process505
   }
   .mbar.fb.menu entryconfigure $wr_proc_index -state disabled
   if [winfo exists .process411] {
      destroyWindow .process411
   }
   .mbar.fb.menu entryconfigure $wr_proc_index -state disabled
   .mbar.fb.menu entryconfigure $wr_data_index -state disabled
   .mbar.fb.menu entryconfigure $wr_pco_index -state disabled
}  ;# closeWindows

####  colorwindows
#
#  Called by the interpreter every context switch to 
#  set the color of boundary of the process windows
#  $curpr   number of current process
#  $lastpr  number of previous process
#  Set the boundary of process $curpr to red unless
#  $lsuspended is -1; else set color to green
#  Set the boundary of process $lastpr to red unless
#  $lsuspended is -1; else set color to orange

proc colorwindows {curpr csuspended lastpr lsuspended} {
   global ShowProcWins
   if {! $ShowProcWins} { return }
   if { ! [winfo exists .process$curpr] } {
      makeprocesswindow $curpr
      tkwait window .process$curpr
   }
   if {$lsuspended != -1} {
      .process$lastpr configure -background red
   } else {
      .process$lastpr configure -background orange
   }
   if {$csuspended != -1} {
      .process$curpr configure -background red
   } else {
      .process$curpr configure -background green
   }
}  ;# colorwindows

####  makeprocesswindow
#
#  Make a window for process $num
#
proc makeprocesswindow {num} {
   global wr_proc_index
   global ShowProcWins
   if {! $ShowProcWins} { return }
   catch {destroy .process$num}
   global .process$num

   toplevel .process$num -class Dialog
   wm title .process$num "Process $num"

   text .process$num.text -relief sunken -bd 2 \
      -yscrollcommand ".process$num.scroll set"
   scrollbar .process$num.scroll -command ".process$num.text yview"
   pack .process$num.scroll -side right -fill y
   pack .process$num.text -fill both -expand yes
   pack .process$num.text -side right -expand 1 -fill both -padx 3m -pady 3m
   wm geometry .process$num 300x250
   .mbar.fb.menu entryconfigure $wr_proc_index -state active
}  ;# makeprocesswindow

####  enablerunbuttons
#
#  Enable the Continue, 1pcode, 1source, Pause buttons
#  of the main menu
#
proc enablerunbuttons {} {
   .bot.continue configure -state active
   .bot.1p configure -state active
   .bot.1s configure -state active
   .bot.pause configure -state active
   .bot.stop configure -state active
}  ;# enablerunbuttons

####  disablerunbuttons
#
#  Disable the Continue, 1pcode, 1source, Pause buttons
#  of the main menu
#
proc disablerunbuttons {} {
   .bot.continue configure -state disabled
   .bot.1p configure -state disabled
   .bot.1s configure -state disabled
   .bot.pause configure -state disabled
   .bot.stop configure -state disabled
}  ;# disablerunbuttons

####  bacirun
#
#   invokes the interpeter through the run command
#   If $runlevel == 0, do a complete intialization
#   If $runlevel == 1, re-run existing pcode file
#
proc bacirun { runlevel } {
   global dir
   global cancelledChoice
   global CurrentPCOFile
   global .process0
   global ShowPCODE
   global quitting
   disable_openfile
   if {"$CurrentPCOFile" != "NULL"} {
      closeProcWindows
      if {([winfo exists .cb])&&($runlevel == 0)} {
         destroy .cb
      }
      makemainwin
      if { $ShowPCODE } {
         makePCOwin 
      }
      .bot.run configure -state disabled
      run [file rootname $CurrentPCOFile] $runlevel
      if {!$quitting} {
         .bot.run configure -state active
      }
   } elseif {! $cancelledChoice} {
      Message "Open a file to run!"
   } else {
      set cancelledChoice 0
   }
   enable_openfile
}  ;# bacirun


#### showsrcln
#
#  Highlight line number $num in source window .source$fno
#
proc showsrcln {fno num} {
   global highlight
   global LastSrcHL
   set w .source$fno
   raise $w
   $w.text configure -state normal
   $w.text tag delete highlight
   $w.text tag add highlight $num.0 $num.end
   $w.text see $num.0
   $w.text tag configure highlight -background yellow
   $w.text configure -state disabled
   set LastSrcHL $fno
}  ;# showsrcln

#### onewinprint
#
#  Put the message $message at the end of process
#  window .process$num
#  
proc onewinprint {message num} {
   global .process$num
   global ShowProcWins
   if [winfo exists .process$num] {
      .process$num.text config -state normal
      .process$num.text insert end  $message
      .process$num.text see end
      .process$num.text config -state disabled
   } elseif {$ShowProcWins} {
      puts stderr ".process$num: $message"
   }
}  ;# onewinprint

#### windowprint
#
#  Put the message $message at the end of
#  process window .process$num. 
#  Make sure user can see message.
#  If $num is not 0, then write $message to .process0, too.
#
proc windowprint {message num} {
   global .process$num

   onewinprint $message $num
   if {$num != 0} {
      onewinprint $message 0
   }
}  ;# windowprint

####  reloadPCODE
#
#  restart the BACI interpreter with
#  the current .pco file
#
proc reloadPCODE {} {
   global CurrentPCOFile
   global cancelledChoice
   global reload_pco_index
   global dir
   set cancelledChoice 0
   closeWindows
   initWindowPosition
   if [winfo exists .source0] {
      wm title .source0 ""
   }
   .bot.run configure -state active
   disablerunbuttons
   if {$CurrentPCOFile == {} } {
      set CurrentPCOFile [findPCOFile $dir]
   }
   if {$cancelledChoice } {
      bacirun 1
   } else {
      bacirun 0
   }
}  ;# reloadPCODE

####  openFile
#
#  Call findPCOFile to select a .pco file to
#  execute, then call bacirun to start the
#  interpreter
#
proc openFile {} {
   global dir
   global CurrentPCOFile
   global cancelledChoice
   global reload_pco_index
   closeWindows
   initWindowPosition
   if [winfo exists .source0] {
      wm title .source0 ""
   }
   .bot.run configure -state active
   disablerunbuttons
   set CurrentPCOFile [findPCOFile $dir]
   .mbar.fb.menu entryconfigure $reload_pco_index -state active
   if {$cancelledChoice } {
      bacirun 1
   } else {
      bacirun 0
   }
}  ;# openFile

#### findPCOFile
#
#  A directory browsing window to allow the user
#  to select a .pco file
#
proc findPCOFile {locdir} {
   global PCOfname
   global cancelledChoice
   global dir
   toplevel .of
   wm title .of "Open a PCODE File"
   frame .of.f
   pack .of.f -side top
   cd $locdir
   set cwd [pwd]
   set dir $cwd 
   frame .of.f0
   label .of.f0.l -width 10 -text "Directory: "
   entry .of.f0.e -width 70 -relief sunken -textvariable tmpfname
   .of.f0.e delete 0 end
   .of.f0.e insert end "$cwd/"
   pack .of.f0.l -side left -in .of.f0 -padx 1 -pady 1 \
      -anchor w -expand true -fill both
   pack .of.f0.e -side left -in .of.f0 -padx 1 -pady 1 \
      -anchor w -expand true -fill both
   pack .of.f0 -side top -expand true -fill both
   button .of.f.cancel -text Cancel -command { \
      set PCOfname NULL
      set cancelledChoice 1
   }
   pack .of.f.cancel -side left -in .of.f -padx 2m -pady 2m -expand 1 -fill x
   pack .of.f -side top -expand 1 -fill x
   scrollbar .of.scroll -command ".of.list yview"
   pack .of.scroll -side right -fill y
   listbox .of.list -yscroll ".of.scroll set" -relief sunken \
      -width 20 -height 10 -setgrid yes
   pack .of.list -side left -fill both -expand yes

   while { 1 } {
      .of.list delete 0 end
      set tmp [exec ls]
      set dirs ""
      foreach u $tmp {
         if { [file isdirectory $u] } {
            lappend dirs "$u/"
         }
      }
      foreach i [concat "../" $dirs [lsort [glob -nocomplain *.pco ] ] ] {
         .of.list insert end $i
      }
      bind .of.list <Double-Button-1> {
         set tmp [.of.list curselection]
         if {$tmp != ""} { set PCOfname [.of.list get $tmp] }
      }
      bind .of.f0.e <Return> {
         if [file exists $tmpfname] {
            if [file isdirectory $tmpfname] {
               set PCOfname $tmpfname
            } elseif { [file extension $tmpfname] != ".pco" } {
               Message "$tmpfname is not a .pco file"
               set tmpfname [pwd]
            } else {
               set PCOfname $tmpfname
            }
         } else {
            if [file exists "$tmpfname.pco"] {
               set PCOfname "$tmpfname.pco"
            } else {
               Message "File $tmpfname does not exist"
               set tmpfname "[pwd]/"
            }
         }
      }
      tkwait variable PCOfname
      if {("$PCOfname" != "NULL")&&([file isdirectory $PCOfname])} {
         if { [catch {cd $PCOfname} tmp] } {
            Message $tmp
         } else {
            set cwd [pwd]
            set dir $cwd 
            .of.f0.e delete 0 end
            .of.f0.e insert end "$cwd/"
         }
      } else {
         destroy .of
         return $PCOfname
      }
   }  ;# while true
}  ;# findPCOFile

####  makesourcewindow
#
#  Create source window .source$num
#  and insert file $fname into it
#
proc makesourcewindow {num fname} {
   global .source$num
   set s .source$num
# array of information about source windows
   global Src$num     
   set S Src$num
# current source window coordinates
   global currSrcWin

   if [winfo exists $s ] {
      wm title $s "$fname"
      return
   }
   toplevel $s -class Dialog
   wm title $s "$fname"

   text $s.text -yscrollcommand "$s.scroll set"
   scrollbar $s.scroll -command "$s.text yview"
   pack $s.scroll -side right -fill y
   pack $s.text -fill both -expand 1
   pack $s.text -side right -expand 1 -fill both 
   wm geometry $s +$currSrcWin(X)+$currSrcWin(Y)
   tkwait visibility $s
   update idletasks
   incr currSrcWin(X) 30
   incr currSrcWin(Y) 30
}  ;# makesourcewindow

   
####  insertFile
#
#  Insert a file named $fname into the source window .source$num
#
proc insertFile {num fname} {
   global dir
   if { [catch {set f [open $fname]} ] } {
      onewinprint "Source file \"$fname\" is not available\n"  0
   } else {
      makesourcewindow $num $fname 
      set s .source$num
      $s.text delete 1.0 end
      set count 1
      $s.text config -state normal
      while {![eof $f]} {
         $s.text insert end "[format "  %3d" $count]  [gets $f]\n"
        incr count
      }
      $s.text configure -state disabled
      raise $s
      close $f
   }  
}  ;# insertFile

####  loadAllFiles
#
# Loads all files given in the infile[] array # read at startup.  
# This proc requires that readtab has run to read the infile table
# and set the last_infile (aka tlast_infile) variable
#
proc loadAllFiles {} {
   global   tlast_infile
   global   infname
   global   curr_file
   set fix $tlast_infile
   if { [info exists infname] } {
      unset infname
   }
   while { $fix >= 0 } {
      set infname($fix) [getInfile $fix]
      insertFile $fix $infname($fix)
      incr fix -1
   }
   set curr_file 0
}  ;# loadAllFiles

######################################################################
#
#  Tcl/Tk main program for the interpreter
#  Run by main.c after the Tcl and Tk interpreters
#  have been initialized
#
######################################################################
# make the interpreter's window
global quitting
set xpause 111
set quitting 0
frame .mbar -relief raised -bd 2
pack .mbar -side top -fill x
if { $InterpPName == "" } {
   set InterpPName "BACI Concurrent PCODE Interpreter"
}
wm title . "$InterpPName"
wm geometry . 570x60+0+0 

setup
initWindowPosition
if { "$CurrentPCOFile" != "NULL" } {
   if {! [string match {*.pco} $CurrentPCOFile] } {
      set CurrentPCOFile "$CurrentPCOFile.pco"
   } 
   bacirun 0
}

# wait here for user to choose exit
tkwait variable quitting 
return 0

# 
# 
#   $Id: menu.tcl,v 1.19 2007/06/01 18:58:44 bynum Exp $
# 
# 

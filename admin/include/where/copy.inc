
<?php

# $Id: copy.inc,v 1.1.1.1 2005/08/10 12:14:03 yoshis Exp $

// Written by James Flemer
// For eGrad2000.com
// <jflemer@alum.rpi.edu>

	/* if survey has been selected ... */
	if(!empty($_GET['sid'])) {
		$sid = intval($_GET['sid']);
		
		$srealm = auth_get_survey_realm($sid);
		
		/* check ACL to see if user is allowed to copy
		 * _this_ survey */
		if($_SESSION['acl']['superuser'] != 'Y' &&
				!auth_is_owner($sid, $xoopsUser->uid()) &&
				!in_array($srealm, array_intersect(
						$_SESSION['acl']['pdesign'],
						$_SESSION['acl']['pall'])) &&
				!auth_no_access(_('to access this survey'))) {
			return;
		}

		/* copy the survey */
		if(!survey_copy($sid)) {
			echo(mkerror(_GT_Error_copying_survey ." (". mysql_error() .")") . "<br>\n");
			echo("<a href=\"". $GLOBALS['ESPCONFIG']['ME'] ."?where=manage\">" . _GT_Go_back_to_Management_Interface . "</a>\n");
			return;
		}
?>
<script language="JavaScript"><!--
window.location="<?php echo($GLOBALS['ESPCONFIG']['ME'] ."?where=manage"); ?>"
//-->
</script>
<?php
		echo("<noscript><a href=\"". $GLOBALS['ESPCONFIG']['ME'] ."?where=manage\">" . _GT_Go_back_to_Management_Interface . "</a></noscript>\n");
		return;
	}

	/* load names and titles of all surveys available to
	 * _this_ user */
	if($xoopsUser->isAdmin()) {
		$sql = "SELECT id,name,title,status,owner,realm FROM ".TABLE_SURVEY." ORDER BY id DESC";
	} else {
		$realms = array_to_insql(
			array_intersect(
				$_SESSION['acl']['pall'],
				array_merge(
					$_SESSION['acl']['pall'],
					$_SESSION['acl']['pdesign'])));
		$sql = "SELECT id,name,title,status,owner,realm FROM ".TABLE_SURVEY.
			" WHERE NOT (status & ". STATUS_DELETED .") AND (owner = '".
			$xoopsUser->uid() ."' || realm $realms) ORDER BY id DESC";
	}
	$result = mysql_query($sql);
	$bg = '';
?>
<h2><?php echo(_GT_Copy_Survey); ?></h2>
<?php echo (_GT_Choose_a_survey);
?>
<table border="0" cellpadding="4" cellspacing="0" align="center" bgcolor="<?php echo($ESPCONFIG['active_bgcolor']); ?>" width="95%">
	<tr bgcolor="#dddddd">
		<th align="left"><?php echo(_GT_ID); ?></th>
		<th align="left"><?php echo(_GT_Name); ?></th>
		<th align="left"><?php echo(_GT_Title); ?></th>
		<th align="left"><?php echo(_GT_Status); ?></th>
		<th align="left"><?php echo(_GT_Owner); ?></th>
		<th align="left"><?php echo(_GT_Group); ?></th>
	</tr>
<?php
	while(list($sid,$name,$title,$status,$owner,$realm) = mysql_fetch_row($result)) {
		if($status & STATUS_DELETED) {
			$stat = _GT_Archived;
		} elseif($status & STATUS_DONE) {
			$stat = _GT_Ended;
		} elseif($status & STATUS_ACTIVE) {
			$stat = _GT_Active;
		} elseif($status & STATUS_TEST) {
			$stat = _GT_Testing;
		} else {
			$stat = _GT_Editing;
		}
		
		if($bg != $ESPCONFIG['bgalt_color1'])
			$bg = $ESPCONFIG['bgalt_color1'];
		else
			$bg = $ESPCONFIG['bgalt_color2'];
?>
	<tr bgcolor="<?php echo($bg); ?>">
		<td><?php echo($sid); ?></td>
		<td><a href="<?php echo("". $GLOBALS['ESPCONFIG']['ME'] ."?where=copy&sid=${sid}"); ?>"><?php echo($name); ?></a></td>

		<td><?php echo($title); ?></td>
		<td><?php echo($stat); ?>
<?
		if( $status & ( STATUS_DELETED | STATUS_DONE ) )
			echo "&nbsp<a href=\"". $GLOBALS['ESPCONFIG']['ME'] ."?where=status&op=r&sid=${sid}\">". _GT_Activate ."</a>";
?></td>
		<td><?php echo($owner); ?></td>
		<td><?php echo($realm); ?></td>
	</tr>
<?php
	}
	mysql_free_result($result);
?>
</table>
<?php echo("<a href=\"". $GLOBALS['ESPCONFIG']['ME'] ."?where=manage\">" . _GT_Go_back_to_Management_Interface . "</a>\n"); ?>

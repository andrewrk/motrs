#!/usr/bin/perl

use strict;

if( @ARGV < 2 ) {
	print "\nUsage:\n\ncompile_props srcfolder destfolder\n\n";
	exit(1);
}

my ($srcfolder, $destfolder) = @ARGV;

opendir(DIR, $srcfolder) || die "Error opening $srcfolder: $!";
my @files = grep( ! /^\./, readdir(DIR));
closedir(DIR);

foreach my $file (@files ){
	my $newname = $file;
	$newname =~ s/\.txt$//i;

	unlink "$destfolder/$newname" if( -e "$destfolder/$newname" ) ;

	
	die "Unable to compile $file\n" if (`perl tools/compile_file.pl "$srcfolder/$file" "$destfolder/$newname"` );
	
}


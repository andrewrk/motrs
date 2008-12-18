#!/usr/bin/perl

use strict;

opendir(DIR, "animations") || die "Error opening animations directory: $!";
my @files = grep( ! /^\./, readdir(DIR));
closedir(DIR);

foreach my $file (@files ){
  unlink "resource/animations/$file.ani" if( -e "resource/animations/$file.ani" ) ;
	compile_animation("animations/$file");
	my $toolEXE = ($^O eq "MSWin32") ? "sjsresource.exe" : "sjsresource";
	`tools/$toolEXE animations/$file resource/animations/$file.ani`;
}


sub compile_animation {
	my $dir = shift;

	if( ! -e "$dir/properties.txt"){
		print "Error processing $dir: missing properties.txt.\n";
		exit(1);
	}
	
	
	# read text props
	my %props;
	open( FILE, "$dir/properties.txt") || die "Error opening $dir/properties.txt: $!";
		while( <FILE> ){
			/^(\w+)\=(.*)$/;
			$props{clean($1)} = clean($2);
		}
	close(FILE);

	# write binary props
	my( $r, $g, $b);
	if( $props{'alphacolor'} =~ m/^(\d+)\,(\d+)\,(\d+)$/ ){
		($r, $g, $b) = ($1, $2, $3);
	} else {
		die "Syntax error in $dir: $props{'alphacolor'} is not a valid value for alphacolor.";
	}
	open(FILE, ">$dir/0") || die "Error opening binary outfile: $!";
	binmode(file);
	print FILE pack("CCCii",$r,$g,$b,$props{'frames'},$props{'delay'});
	close(FILE);
}

sub clean {
	my $str = shift;
	$str =~ s/[\n\r]//g;
	return $str;
}


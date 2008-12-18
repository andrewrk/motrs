#!/usr/bin/perl

use strict;

if( @ARGV < 2 ){
	print "\nUsage:\n\ncompile_file in.txt out.prop\n\n";
	exit(1);
}

my %propType = ( #string, color, int, float, bool, directionstring, maplink
					"walking"=>["directionstring", ""],
					"running"=>["directionstring", ""],
					"standing"=>["directionstring", ""],
					"alpha"=>["color","none"],
					"bitmap"=>["string","none"],
					"obstacle"=>["bool", "false"],
					"feetoffsetx"=>["int", 0],
					"feetoffsety"=>["int", 0],
					"feetwidth"=>["int", 0],
					"feetheight"=>["int", 0],
					"speed"=>["float", 0],
					"visible"=>["bool", "true"],
					"portal"=>["maplink", "none"],
					"ground"=>["enum", "normal", {normal=>0, water=>1, mud=>2} ]
					);

compile_file($ARGV[0], $ARGV[1]);


sub compile_file {
	my($in,$out) = @_;
	
	my %props;
	open(FILE, $in) || die "Error opening $in: $!";
	while(<FILE>){
		$props{clean($1)}=clean($2) if( /^(\w+)\=(.*)$/ );
	}
	close(FILE);
	
	# delete pointless entries
	foreach my $key (keys %props) {
		delete $props{$key} if( $props{$key} eq $propType{$key}->[1] );
	}
	my $numProps = scalar(keys %props);
	
	
	open(OUT, ">$out") || die "Error opening $out: $!";
	binmode(OUT);
	print OUT pack("i", 1); #version
	print OUT pack("i", $numProps ); # number properties
	foreach my $key (keys %props){
		store_string($key);
		if( defined $propType{$key} ){
			if( $propType{$key}->[0] eq 'string' ){
				store_string($props{$key});
			} elsif( $propType{$key}->[0] eq 'color' ){
				$props{$key} =~ m/^\s*(\d+)\s*\,\s*(\d+)\s*\,\s*(\d+)\s*$/ || die "Bad format: $key=$props{$key}\n";
				print OUT pack("i", 3); # takes 3 bytes to store color
				print OUT pack("CCC", $1, $2, $3);
			} elsif( $propType{$key}->[0] eq 'int'){
				print OUT pack("i", 4); # takes 4 bytes to store int
				print OUT pack("i", $props{$key});
			} elsif( $propType{$key}->[0] eq 'float'){
				print OUT pack("i", 4); # takes 4 bytes to store float
				print OUT pack("f", $props{$key});
			} elsif( $propType{$key}->[0] eq 'bool'){
				print OUT pack("i", 1); #takes 1 byte to store boolean
				print OUT pack("C", $props{$key} eq "true" ? 1 : 0 );
			} elsif( $propType{$key}->[0] eq 'directionstring' ) {
				my @strs = trim(split(/,/,$props{$key}));
				
				my $sum = 0;
				foreach(@strs){
					$_ = "" if( lc($_) eq "null" || lc($_) eq "none");
					$sum += length($_);
				}
				
				print OUT pack("i", scalar(@strs) * 4 + $sum);
				
				map { store_string($_); } @strs;
			} elsif ( $propType{$key}->[0] eq 'maplink'){
				my @strs = trim(split(/,/,$props{$key}));
				
				print OUT pack("i", 4 + length($strs[0]) + 4 * 2);
				store_string($strs[0]);
				print OUT pack("ii", $strs[1], $strs[2] );
			} elsif( $propType{$key}->[0] eq 'enum') {
				print OUT pack("ii", 4, $propType{$key}->[2]->{$props{$key}});
			} else {
				die "ERROR: Unsupported property type: $propType{$key}->[0]\n";
			}
		} else {
			die "ERROR: Unknown property: '$key'\n";
		}
	}
	close(OUT);
	
}


sub clean {
	my $str = shift;
	$str =~ s/[\n\r]//g;
	return $str;
}

sub trim {
	my @args = @_;
	s/^\s*(.*?)\s*$/$1/g foreach(@args);
	return @args;
}

sub store_string {
	my $string = shift;
	
	
	print OUT pack( "i" , length($string) );
	
	map { print OUT pack( "C" , ord($_) ) } split(//, $string);
		
}

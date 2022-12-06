function [var] = ti2matlab(file);
% Nacte soubor 'file' ulozeny v prostredi Code Composer a ulozi ho do promenne 
% 'var'.

fin = fopen( file, 'r');
if( fin == -1)
    return;
end;

fgets( fin);
pom = fscanf( fin, '0x%4X%4X\n');
fclose( fin);
pom(pom > 32767) = pom(pom > 32767) - 65536;
sudy = 2:2:length( pom); 
lichy = sudy - 1;

var = zeros( length( sudy), 2);
var( :,1) = pom( lichy);
var( :,2) = pom( sudy);
var = var/32768;
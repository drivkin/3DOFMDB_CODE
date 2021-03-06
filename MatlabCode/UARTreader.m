s=serial('COM9','BaudRate',115200,'InputBufferSize',1000000);
%%
fopen(s);

%%
a=fread(s,1000000/4,'float');
fclose(s);



%% Currently for testing
mlength = 4;

n1 = a(1:mlength:length(a));
n2 = a(2:mlength:length(a));
n3 = a(3:mlength:length(a));
% DCC = a(4:mlength:length(a));
% sector = a(5:mlength:length(a));
% Va = a(6:mlength:length(a));
% Vb = a(7:mlength:length(a));
t = a(4:mlength:length(a));


%%
%now process t to get rid of loop around
tcorr = t;
for i=2:length(t)
    temp = t(i)-t(i-1);
    if(temp >= 0)
        tcorr(i) = tcorr(i-1)+temp;
    else
        tcorr(i) = tcorr(i-1)+(65536+temp);
    end
end
tcorr = tcorr*(2^-16);
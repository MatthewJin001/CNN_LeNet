function [] = convertData2Img(SAMPLE,path)

 if ~exist(path) 
    mkdir(path)         % �������ڣ��ڵ�ǰĿ¼�в���һ����Ŀ¼��Figure��
 end 

[m,n,l] = size(SAMPLE);
image = zeros(m,n);
for iIndex = 1:l    
    image = SAMPLE(:,:,iIndex); 
    %iNameIndex = int2str(iIndex);
    name = [path, '/', int2str(iIndex),'.bmp'];    
    imwrite(image,name);
end
    



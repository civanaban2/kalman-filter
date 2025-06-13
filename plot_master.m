f = figure('Position', [100, 100, 1200, 800]);
hold on;
xlabel('X'); ylabel('Y');
title('Gerçek Zamanlı Ölçüm Grafiği');
axis equal;
zoom on;

t = linspace(0, 80, 2000);
x_ref = 100 + 5*t;
y_ref = 200 + 3*t;
h_ref = plot(x_ref, y_ref, 'b-', 'DisplayName', 'Gerçek hareket');

h_meas = plot(NaN, NaN, 'r*', 'DisplayName', 'Ölçümler');

legend([h_ref, h_meas]);

last_n = 0;
while true
    if isfile('log.txt')
        data = readmatrix('log.txt');

        if size(data,2) >= 3 && size(data,1) > last_n
            new_rows = data(last_n+1:end, :);
            x_new = new_rows(:,2);
            y_new = new_rows(:,3);

            plot(x_new, y_new, 'r*');
            drawnow;
            last_n = size(data,1);
        end
    end
    pause(1);
end

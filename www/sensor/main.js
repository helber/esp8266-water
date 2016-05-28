var erro_1 = 17;
var erro_2 = 17;
var dist_cheio1 = 30;
var dist_vazio1 = 97;
var dist_cheio2 = 27;
var dist_vazio2 = 102;

function load_append() {

    $.getJSON('/sensor', {})
    .done(function(data) {
        var s1 = data['sonar1'];
        var s2 = data['sonar2'];
        if ( s1 < erro_1 || s1 > dist_vazio1 ) {
            console.log('Erro S1 ' + s1);
            $('#status').text('ERROR S1=' + s1);
            setTimeout(load_append, 1000);
            return;
        }
        if ( s2 < erro_2 || s2 > dist_vazio2 ) {
            console.log('Erro S2 ' + s2);
            $('#status').text('ERROR S2=' + s2);
            setTimeout(load_append, 1000);
            return;
        }
        var vp1 = (dist_vazio1 - s1) / (dist_vazio1 - dist_cheio1);
        var vp2 = (dist_vazio2 - s2) / (dist_vazio2 - dist_cheio2);
        var pp1 = Math.round(vp1 * 100);
        var pp2 = Math.round(vp2 * 100);
        if (vp1 > 1) vp1 = 1;
        if (vp2 > 1) vp2 = 1;
        var t1 = Math.round((1 - vp1) * 100);
        var t2 = Math.round((1 - vp2) * 100);

        $('#s1_in').text('Caixa 1 = ' + pp1 + ' % (' + s1 + ' cm)');
        $('#s2_in').text('Caixa 2 = ' + pp2 + ' % (' + s2 + ' cm)');
        $('#status').text('OK');
        $('#s1_in').css('top', t1 + '%');
        $('#s2_in').css('top', t2 + '%');
        setTimeout(load_append, 10000);
    })
    .fail(function(jqxhr, textStatus, error) {
        $('#status').text('ERRO');
    })

}

$( document ).ready(function() {
    $('body').append('<div id="status">????</div>');
    $('#s1').append('<div id="s1_in">S1</div>');
    $('#s2').append('<div id="s2_in">S2</div>');
    load_append();
    // setTimeout(load_append, 10000);
})

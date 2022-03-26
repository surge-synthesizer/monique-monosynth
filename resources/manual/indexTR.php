<?php 

// ########################################################################################
// ########################################################################################
// ########################################################################################
$lang = "en";
if(isset($_GET["lang"]))
{
    $lang = substr($_GET["lang"], 0, 2);
}
else
{
    $lang = substr($_SERVER['HTTP_ACCEPT_LANGUAGE'], 0, 2);
}

// ########################################################################################
// ########################################################################################
// ########################################################################################
// HTML Title, HTML Description
$title =        "B-Step Sequencer - The harmonic Step Sequencer and Arpeggiator";
$description =  "The music step sequencer plugin to create your melodies, arpeggios and bass lines on the fly. VST, AU (MIDI FX), Standalone, iPad, Android, Windows, Mac & Linux.";
$keywords =     "Sequencer, VST Plugin, Sequencer Plugin, VST Sequencer";

if($lang == "de") 
{
    $title =        "B-Step Sequencer - Der harmonische Step Sequencer und Arpeggiator";
    $description =  "Mit diesem Sequencer erstellst Du im Handumdrehen neue Melodien, Loops, Arpeggios und Basslines. Plugin: VST, AU - Windows, Mac & Linux - iPad & Android.";
    $keywords =     "Sequencer, VST Plugin, Sequencer Plugin, VST Sequencer";
}
else if($lang == "fr") 
{
}
else if($lang == "es") 
{
}

// ########################################################################################
// ########################################################################################
// ########################################################################################
// LINKS
$highlights = "Highlights";
$overview = "Overview";
$samples = "Songs";
$pricing = "Pricing";
$about = "About";
$magazine = "Videos &amp; Tutorials";
$contact = "Contact";
$top = "Top";
$manual_en = "Manual (EN)";
$manual_de = "Manual (DE)";
$dl_link = "Download";

if($lang == "de") 
{
    $highlights = "Highlights";
    $overview = "Übersicht";
    $samples = "Beispiele";
    $pricing = "Preise";
    $about = "About";
    $magazine = "Videos &amp; Tutorials";
    $contact = "Kontakt";
    $top = "Oben";
    $manual_en = "Handbuch (EN)";
    $manual_de = "Handbuch (DE)";
    $dl_link = "Download";
}
else if($lang == "fr") 
{
    $highlights = "Highlights";
    $overview = "Aperçu";
    $samples = "Morceaux";
    $pricing = "Prix";
    $about = "A Propos De";
    $magazine = "Vidéos & Tutoriels";
    $contact = "Contact";
    $top = "TODO"; // TODO!!
    $manual_en = "Manual (EN)";
    $manual_fr = "Manual (FR)";	
    $manual_de = "Manual (DE)";
    $dl_link = "TODO"; // TODO!!
}
else if($lang == "es") 
{
}

// ########################################################################################
// ########################################################################################
// ########################################################################################
// MISC 
$misc_video_error = "To view this video please enable JavaScript, and consider upgrading to a web browser that<a href='http://videojs.com/html5-video-support/' target='_blank'>supports HTML5 video</a>";
$misc_musician =    "Musician";

if($lang == "de") 
{
    $misc_video_error = "Um dieses Video anzusehen muss JavaScript aktiviert sein oder Du besorgst Dir einen <a href='http://videojs.com/html5-video-support/' target='_blank'>HTML5 kompatiblen Browser</a>";
    $misc_musician =    "Musiker";
}
else if($lang == "fr") 
{
}
else if($lang == "es") 
{
}

// ########################################################################################
// ########################################################################################
// ########################################################################################
// Header
$h1 =               "<span class='blue'>Arpeggios</span> and <span class='orange'>Melodies</span>, created in Seconds";
$slogan =           "The harmonic music step sequencer to create your melodies, arpeggios and bass lines on the fly, now everyone can be pro.";
$dl_button_top =    "Download Free Demo"; 

if($lang == "de") 
{
$h1 =               "Perfekt geeignet für <span class='blue'>Melodien</span> und <span class='orange'>Arpeggios</span>.";
$slogan =           "B-Step Sequencer rückt Deine Musik im Vordergrund - pack den Spass aus und lass die ganze Theorie kurz hinter dir.";
$dl_button_top =    "Download Free Demo"; 
}
else if($lang == "fr") 
{
}
else if($lang == "es") 
{
}

// ########################################################################################
// ########################################################################################
// ########################################################################################
// PICTURE ALTERNATIVE TEXT
// SEO FOCUS!!! - FUCK CORRECTNESS :D
$alt_vst2 = "vst compatible sequencer plugin";
$alt_vst3 = "vst3 compatible sequencer plugin";
$alt_au = "audio unit or logic compatible sequencer plugin";
$alt_standalone = "standalone software sequencer";
$alt_windows = "a sequencer for windows";
$alt_mac = "a sequencer for mac os";
$alt_linux = "a sequencer for linux";
$alt_ios = "a sequencer for ios ipad";
$alt_raspberry = "a sequencer for raspberry pi";
$alt_android = "a sequencer for android";
$alt_ui = "User Interface of B-Step Sequencer the VST step sequencer plugin";

if($lang == "de") 
{
    $alt_vst2 = "vst kompatibles sequencer plugin";
    $alt_vst3 = "vst3 kompatibles sequencer plugin";
    $alt_au = "audio unit oder logic kompatibles sequencer plugin";
    $alt_standalone = "standalone software sequencer";
    $alt_windows = "ein sequencer für windows";
    $alt_mac = "ein sequencer für mac os";
    $alt_linux = "ein sequencer für linux";
    $alt_ios = "ein sequencer for ios ipad";
    $alt_raspberry = "ein sequencer for raspberry pi";
    $alt_android = "ein sequencer for android";
    $alt_ui = "User Interface von B-Step dem VST step sequencer plugin";
}
else if($lang == "fr") 
{
}
else if($lang == "es") 
{
}

// ########################################################################################
// ########################################################################################
// ########################################################################################
// $H2_1 , Slogan_1
$h2_1 =     "Simplicity and complexity fused into one Music composing Tool";
$slogan_1 = "B-Step offers you only the features you need, but if you like you can enable and dive deep into a feature rich world.";

if($lang == "de") 
{
    $h2_1 =     "Mit diesem Sequencer komponierst Du extrem schnell.";
    $slogan_1 = "B-Step zeigt Dir nur was Du brauchst - oder 5 Sterne Deluxe: Features so weit das Auge reicht!";
}
else if($lang == "fr") 
{
}
else if($lang == "es") 
{
}

// ########################################################################################
// ########################################################################################
// ########################################################################################
// Point 1
$point_1_h =      "Melodies";
$point_1_slogan = "Whether monophonic or polyphonic, <strong>chord based programming </strong>keeps your focus only on finding the <strong>right sequence.</strong>";
    
if($lang == "de") 
{
$point_1_h =      "Melodien";
$point_1_slogan = "Egal ob monophon oder polyphon, <strong>Akkordbasierende</strong> Programmierung <strong>klingt einfach immer</strong>.";
}
else if($lang == "fr") 
{
}
else if($lang == "es") 
{
}

// ########################################################################################
// ########################################################################################
// ########################################################################################
// Point 2
$point_2_h =      "Progressions";
$point_2_slogan = "<strong>1-5-3 or 1-3-5? </strong>No problem, B-Step Sequencer makes it so easy to find the harmony for your next song.";

if($lang == "de") 
{
    $point_2_h =      "Akkordfolgen";
    $point_2_slogan = "<strong>1-5-3 oder 1-3-5? </strong>Kein Problem, mit B-Step findest du die passende Harmonie für deinen nächsten Song.";
}
else if($lang == "fr") 
{
}
else if($lang == "es") 
{
}

// ########################################################################################
// ########################################################################################
// ########################################################################################
// Point 3
$point_3_h =      "Experimental";
$point_3_slogan = "B-Step offers you so many possibilities that you can solve standard things on <strong>more than one way.</strong>";

if($lang == "de") 
{
    $point_3_h =      "Experimental";
    $point_3_slogan = "Bei B-Step <strong>stimmts einfach immer</strong>. Einmal hier und da gedreht und schon ist die <strong>neue Sequence fertig</strong>!";
}
else if($lang == "fr") 
{
}
else if($lang == "es") 
{
}

// ########################################################################################
// ########################################################################################
// ########################################################################################
// Point 4
$point_4_h =      "Plugin, Standalone, Tablet";
$point_4_slogan = "Available for <strong>Mac OS X, Windows &amp; Linux, Raspberry, iPad, Android</strong> as <strong>Standalone, VST</strong> and <strong>AudioUnit (AU)</strong> plugin.";

if($lang == "de") 
{
$point_4_h =      "Plugin, Standalone, Tablet";
$point_4_slogan = "Läuft! <strong>Mac OS X, Windows &amp; Linux, Raspberry, iPad, Android</strong> als <strong>Standalone, VST</strong> und <strong>AudioUnit (AU)</strong>.";
}
else if($lang == "fr") 
{
}
else if($lang == "es") 
{
}

// ########################################################################################
// ########################################################################################
// ########################################################################################
// $H2_2 , Slogan_2
$h2_2 =     "Highlights of B-Step Sequencer";
$slogan_2 = "B-Step Sequencer is designed for live usage, jam sessions and studio productions.";

if($lang == "de") 
{
$h2_2 =     "B-Step Sequencer Highlights";
$slogan_2 = "Dieser Sequencer fühlt sich nicht nur im Studio gut an - er freut sich auf jede Session.";
}
else if($lang == "fr") 
{
}
else if($lang == "es") 
{
}

// ########################################################################################
// ########################################################################################
// ########################################################################################
// FEATURE LEFT
$feature_left_h =    "Melodic, Harmonic and Groovy";
$feature_left_body = 
"<p>B-Step protects you from errors.</p>
<ul>
    <li>Just select or load a few matching chords for your project an keep forced in harmony</li>
    <li>Create a whole sequence with just one step repeat and a hidden note-find-filter</li>
    <li>Quick arrangement by repeating bars or parts of a pattern multiple times</li>
    <li>With B-Step's shuffle feature you get the right groove for your piece</li>
    <li>Octave offsets per step-, per bar- or track keeps you session varied</li>
</ul>";

if($lang == "de") 
{
    $feature_left_h =    "Klingt, Passt - Grooved";
    $feature_left_body = 
    "<p>B-Step hilft Fehler zu vermeiden.</p>
    <ul>
        <li>Sind die Akkorde erst eingestellt oder aus den Presets geladen kann nichts mehr schief klingen</li>
        <li>Mit nur einem Step und ein paar Repeats lässt sich eine ganze Sequence stricken</li>
        <li>Schnelles Arrangement durch Takt- oder partielle Wiederholungen</li>
        <li>Das Shuffle Feature sorgt für den richtigen Groove in Deinem Set</li>
        <li>Mit Oktave Offsets pro Step-, Takt- oder Track/Saite kannst Du Dein Stück einfach spannend halten</li>
    </ul>";
}
else if($lang == "fr") 
{
}
else if($lang == "es") 
{
}

// ########################################################################################
// ########################################################################################
// ########################################################################################
// FEATURE RIGHT
$feature_right_h =    "Live - Usage";
$feature_right_body = 
"<p>Whenever touch-, mouse- or MIDI remote control, you'll feel at home.</p>
<ul>
    <li>Copy a bar or anything else multiple times in a simple single multi drag action</li>
    <li>Pattern clipboards allows you to reuse a pattern with different or same settings</li>
    <li>Special support for one or two Novation launchpads (1st gen, update in progress)</li>
    <li>Whatever you want either it is light day or night - customize the user interface to your needs</li>
</ul>";

if($lang == "de") 
{
    $feature_right_h =    "Live Tauglich";
    $feature_right_body = 
    "<p>B-Step lässt sich mit Maus, Touchscreen oder MIDI Controller steuern.</p>
    <ul>
        <li>Ganze Takte lassen sich mit einer einfachen DragAktion mehrfach kopieren</li>
        <li>Über TaktSnapshots lassen sich bestehende Takte erneut nutzen</li>
        <li>Extra unterstützt: MIDI Map für bis zu zwei Novation Launchpads</li>
        <li>Ob Tag oder Nacht, B-Step's UI lässt sich an Deine Bedürfnisse farblich anpassen</li>
    </ul>";
}
else if($lang == "fr") 
{
}
else if($lang == "es") 
{
}

// ########################################################################################
// ########################################################################################
// ########################################################################################
// $H2_3 , Slogan_3
$h2_3 =     "Sequencer User Feedback";
$slogan_3 = "What other users about B-Step Sequencer say.";

if($lang == "de") 
{
    $h2_3 =     "Sequencer Feedback";
    $slogan_3 = "Was andere Nutzer über B-Step Sequencer sagen.";
}
else if($lang == "fr") 
{
}
else if($lang == "es") 
{
}

// ########################################################################################
// ########################################################################################
// ########################################################################################
// PRICING
$pricing_h =    "Fair Trade - test first and buy if you Like";
$pricing_slogan = "B-Step Sequencer comes as a Demo version you can test with indefinitely!";

if($lang == "de") 
{
    $pricing_h =    "Fair Trade - erst anfassen, dann kaufen.";
    $pricing_slogan = "Mit ein paar Einschränkungen lässt sich die B-Step Sequencer Demo für unbestimmte Zeit testen.";
}
else if($lang == "fr") 
{
    $pricing_h =    "Testez la et achetez la après satisfaction.";
    $pricing_slogan = "B-Step est disponible en version Démo vous pouvez la tester indéfiniment!";
}
else if($lang == "es") 
{
}

// ########################################################################################
// ########################################################################################
// ########################################################################################
// DEMO
$demo_h =     "Free";
$demo_price = "$0";
$demo_body = 
"<ul class='pro-con'>
    <li>Test all features</li>
    <li class='con'>Limits: no saving</li>
</ul>";
$demo_dl   =  "Free Download";
$demo_dv =    "Demo Version";

if($lang == "de") 
{
    $demo_h =     "Free";
    $demo_price = "0€";
    $demo_body =  
    "<ul class='pro-con'>
        <li>Test aller Features</li>
        <li class='con'>Limits: kein Speichern</li>
    </ul>";
    $demo_dl   =  "Free Download";
    $demo_dv =    "Demo Version";
}
else if($lang == "fr") 
{
    // TODO replacement for "FREE"?
    $demo_h =     "Free";
    $demo_price = "0€";
    $demo_body =  
    "<ul class='pro-con'>
        <li>Testez la</li>
        <li class='con'>Limitations: pas de sauvegarde</li>
    </ul>";
    $demo_dl   =  "Téléchargement gratuit";
    $demo_dv =    "Version Démo";
}
else if($lang == "es") 
{
}

// ########################################################################################
// ########################################################################################
// ########################################################################################
// PROFESSIONELL
$pro_h =     "Hobby &amp; Musicians";
$pro_price = "$49";
$pro_body = 
"<ul class='pro-con'>
    <li>Everything In Demo, Including:</li>
    <li>Free lifetime support</li>
    <li>Audio Export / Recording (VST)</li>
    <li>Load and Save all your settings, projects, chord sets and color themes</li>
</ul>";
$pro_dl   =  "Buy Now";

if($lang == "de") 
{
    $pro_h =     "Hobby &amp; Musiker";
    $pro_price = "39€";
    $pro_body =  
    "<ul class='pro-con'>
        <li>Alles wie im Demo, Plus:</li>
        <li>Free lifetime support</li>
        <li>Laden und Speichern all Deiner Einstellungen, Projekte, Farbthemen und ChordSets</li>
    </ul>";
    $pro_dl   =  "Jetzt Kaufen";
}
else if($lang == "fr") 
{
}
else if($lang == "es") 
{
}

// ########################################################################################
// ########################################################################################
// ########################################################################################
// Download
$download_h =    "Download the Demo version of this Sequencer";

if($lang == "de") 
{
    $download_h =    "Zieh Dir Deine B-Step Sequencer Demo Version";
}
else if($lang == "fr") 
{
}
else if($lang == "es") 
{
}

// ########################################################################################
// ########################################################################################
// ########################################################################################
// OVERVIEW - ATTENTION, this is the most tricky section of this website and we have to take care that we keep the length of each point similarly to keep the layout as it is.
$gear_title = "Sequencer Gear Overview";
$gear_slogan = "B-Step Sequencer is a powerful music step sequencer for professionals and newcomers.";
    
$gear_seq = "Sequencer Features";
$gear_seq_1 = "shuffle / swing with optional volume dumping";
$gear_seq_2 = "sequencer ratcheting or step repeats";
$gear_seq_3 = "monophonic and polyphonic playback modes";
$gear_seq_4 = "pattern auto chaining";
$gear_seq_5 = "reverse playback";
$gear_seq_6 = "different note measurements";
$gear_seq_7 = "step probability";
$gear_seq_8 = "pattern position modifying";
$gear_seq_9 = "partial bar repeats";
$gear_seq_10 = "send control and program changes at each step";
$gear_seq_11 = "random chords from scales";
$gear_seq_12 = "16 pattern with 4 tracks and 16 steps";
$gear_seq_13 = "split it in up to 4 sub sequencers";

$gear_ui = "User Interface";
$gear_ui_1 = "free scalable (desktop computers)";
$gear_ui_2 = "10 finger multi-touch support";
$gear_ui_3 = "clean, purisic and customizable colors";
$gear_ui_4 = "embedded and online user manual (Pdf in progress)";
$gear_ui_5 = "multi-copy dragging";
$gear_ui_6 = "clipboard for pattern snapshots";

$gear_tech = "Technology";
$gear_tech_1 = "VST, AU MIDI FX (Logic only), Standalone";
$gear_tech_2 = "Windows XP+, Mac 10.5+, Linux";
$gear_tech_3 = "iOS 7.0+, Android 3.2 or newer";
$gear_tech_4 = "Raspberry Pi";
$gear_tech_5 = "MIDI In, MIDI Out";
$gear_tech_6 = "virtual port support (not Windows)";

$gear_data = "Data Management";
$gear_data_1 = "auto save/load last session";
$gear_data_2 = "import/export of your data (via mail or text)";
$gear_data_3 = "download projects from our repository";

$gear_midi = "MIDI Controller";
$gear_midi_1 = "Native Novation Launchpad support (for up to two Launchpads)";
$gear_midi_2 = "Advanced MIDI mapping";

$gear_spec = "MIDI Specification";
$gear_spec_1 = "clock master (up to 400 BPM)";
$gear_spec_2 = "clock slave mode";
$gear_spec_3 = "5x MIDI OUT to control up to 5 hardware or software synthesizers";
$gear_spec_4 = "1x MIDI IN for sync to another master";
$gear_spec_5 = "1x IN &amp; OUT for MIDI remote controller";
$gear_spec_6 = "2x IN &amp; OUT for Novation Launchpad";
$gear_spec_7 = "MIDI clock thru and other routing options";

if($lang == "de") 
{
}
else if($lang == "fr") 
{
}
else if($lang == "es") 
{
}

?>

<!DOCTYPE html>
<html lang="<?php echo $lang ?>">
  <head>
    <title><?php echo $title ?></title>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta name="description" content="<?php echo $description ?>">    
    <meta name="author" content="Monoplugs">
    <meta name="copyright" content="© 2016 Thomas Arndt | Monoplugs.com">
    <meta http-equiv="last-modified" content="2016-4-18@22:00:00 CET">
    <meta name="keywords" content="<?php echo $keywords ?>">
    <link rel="stylesheet" href="css/style.css">
    <link rel="icon" href="favicon_.ico">
    <link rel="canonical" href="http://b-step.monoplugs.com?lang=<?php echo $lang ?>">
    <style>
        .btn-cont img{margin:5px; margin-top:25px}.grey{opacity: 0.4;filter: alpha(opacity=40);background-color: #000;}
    </style>
  </head>
  <body>
    <header id="site-hero">
      <div class="container">
        <nav class="site-nav group"><a href="?lang=<?php echo $lang ?>" class="brand">B-Step Sequencer</a>
          <ul>
            <li>&nbsp;</li>
            <li><a href="?lang=de"><img <?php if($lang!="de") echo "class='grey'"; ?> src="flags/de.png" title="Deutsch / German" alt="Deutsch / German"></a></li>
            <li><a href="?lang=en"><img <?php if($lang!="en") echo "class='grey'"; ?> src="flags/en.png" title="English" alt="English"></a></li>
            <li><a href="?lang=fr"><img <?php if($lang!="fr") echo "class='grey'"; ?> src="flags/fr.png" title="Français / French" alt="Français / French"></a></li>
            <!--<li><a href="?lang=es"><img src="flags/es.png"></a></li>-->
            <li></li>
          </ul>
          <ul>
            <li><a href="#highlights"><?php echo $highlights ?></a></li>
            <li><a href="#overview"><?php echo $overview ?></a></li>
            <li><a href="#pricing"><?php echo $pricing ?></a></li>
            <!--<li><a href="#downloads">Download</a></li>-->
            <!--<li><a href="/manual">Manual ></a></li>-->
            <!--<li><a href="#about">About</a></li>
            <li><a href="http://monoplugs.com/magazine/category/b-step-sequencer/"><?php echo $magazine ?></a></li>-->
          </ul>
        </nav>
        <hgroup>
          <h1 class="title"><?php echo $h1 ?></h1>
          <h2 class="slogan"><?php echo $slogan ?></h2>
        </hgroup>
        <div class="btn-cont"><a href="#downloads" class="button bstep-button"><span class="fa fa-download"></span><?php echo $dl_button_top ?></a>
            <br>
            <h3>
              <img src="/img/logos/vst2.png" alt="<?php echo $alt_vst2 ?>">
              <img src="/img/logos/au.png" alt="<?php echo $alt_au ?>">
              <img src="/img/logos/midi.png" alt="<?php echo $alt_standalone ?>">
              <span style="margin:20px"></span>
              <img src="/img/logos/windows.png" alt="<?php echo $alt_windows ?>">
              <img src="/img/logos/apple.png" alt="<?php echo $alt_mac ?>">
              <img src="/img/logos/linux.png" alt="<?php echo $alt_linux ?>">
              <img src="/img/logos/ios-col.png" alt="<?php echo $alt_ios ?>">
              <img src="/img/logos/android-col.png" alt="<?php echo $alt_android ?>">
              <img src="/img/logos/raspberry-pi-col.png" alt="<?php echo $alt_raspberry ?>">
            </h3>
        </div>
        <div class="laptop-screen mid-only"><img src="img/video.png" alt="<?php echo $alt_ui ?>"></div>
      </div>
    </header>
    <section id="points">
      <div class="laptop-screen xs-only"><img src="img/video.png" alt="<?php echo $alt_ui ?>"></div>
      <h2 class="section-heading"><?php echo $h2_1 ?></h2>
      <p class="section-sub"><?php echo $slogan_1 ?></p>
      <div class="point-container group">
        <div class="point"><span class="fa fa-bell"></span>
          <h3><?php echo $point_1_h ?></h3>
          <p><?php echo $point_1_slogan ?></p>
        </div>
        <div class="point"><span class="fa fa-music"></span>
          <h3><?php echo $point_2_h ?></h3>
          <p><?php echo $point_2_slogan ?></p>
        </div>
        <div class="point"><span class="fa fa-flask"></span>
          <h3><?php echo $point_3_h ?></h3>
          <p><?php echo $point_3_slogan ?></p>
        </div>
        <div class="point"><span class="fa fa-unlock-alt"></span>
          <h3><?php echo $point_4_h ?></h3>
          <p><?php echo $point_4_slogan ?></p>
        </div>
      </div>
    </section>
    <section id="highlights">
      <h2 class="section-heading"><?php echo $h2_2 ?></h2>
      <p class="section-sub"><?php echo $slogan_2 ?></p>
      <div class="highlights-container container group">
        <div class="highlight">
          <div class="highlight-video">
            <video controls="" preload="none" width="530" height="400" poster="img/groovy.jpg" data-setup="{ &quot;aspectRatio&quot;:&quot;553:350&quot;, &quot;playbackRates&quot;: [1, 1.5, 2] }" class="video-js">
              <source src="http://monoplugs.com/public/dl/b-step-sequencer-arpeggio.mp4" type="video/mp4">
              <p class="vjs-no-js"><?php echo $misc_video_error ?></p>
            </video>
          </div>
          <div class="highlight-content">
            <h3><?php echo $feature_left_h ?></h3>
            <?php echo $feature_left_body ?>
          </div>
        </div>
        <div class="highlight">
          <div class="highlight-video">
            <video controls="" preload="none" width="530" height="400" poster="img/usage.jpg" data-setup="{ &quot;aspectRatio&quot;:&quot;553:350&quot;, &quot;playbackRates&quot;: [1, 1.5, 2] }" class="video-js">
              <source src="http://monoplugs.com/public/dl/b-step-sequencer-live.mp4" type="video/mp4">
              <p class="vjs-no-js"><?php echo $misc_video_error ?></p>
            </video>
          </div>
          <div class="highlight-content">
            <h3><?php echo $feature_right_h ?></h3>
            <?php echo $feature_right_body ?>
          </div>
        </div>
      </div>
    </section>
    <section id="overview">
      <h2 class="section-heading"><?php echo $gear_title ?></h2>
      <p class="section-sub"><?php echo $gear_slogan ?></p>
      <div class="specs-container">
        <div class="mobile-only spec-img"><img src="img/b-step-sequencer.jpg" alt="<?php echo $alt_ui ?>"></div>
        <div class="spec-img pc-only"><img src="img/specification.png" alt="<?php echo $alt_ui ?>"></div>
        <div class="specs">
          <div class="group">
            <div class="spec left sequencer-features">
              <h2 class="spec-title"><?php echo $gear_seq ?></h2>
              <ul class="spec-list">
                <li><?php echo $gear_seq_1 ?></li>
                <li><?php echo $gear_seq_2 ?></li>
                <li><?php echo $gear_seq_3 ?></li>
                <li><?php echo $gear_seq_4 ?></li>
                <li><?php echo $gear_seq_5 ?></li>
                <li><?php echo $gear_seq_6 ?></li>
                <li><?php echo $gear_seq_7 ?></li>
                <li><?php echo $gear_seq_8 ?></li>
                <li><?php echo $gear_seq_9 ?></li>
                <li><?php echo $gear_seq_10 ?></li>
                <li><?php echo $gear_seq_11 ?></li>
                <li><?php echo $gear_seq_12 ?></li>
                <li><?php echo $gear_seq_13 ?></li>
              </ul>
            </div>
            <div class="spec left ui">
              <h2 class="spec-title"><?php echo $gear_ui ?></h2>
              <ul class="spec-list">
                <li><?php echo $gear_ui_1 ?></li>
                <li><?php echo $gear_ui_2 ?></li>
                <li><?php echo $gear_ui_3 ?></li>
                <li><?php echo $gear_ui_4 ?></li>
                <li><?php echo $gear_ui_5 ?></li>
                <li><?php echo $gear_ui_6 ?></li>
              </ul>
            </div>
          </div>
          <div class="group">
            <div class="spec left technology">
              <h2 class="spec-title"><?php echo $gear_tech ?></h2>
              <ul class="spec-list">
                <li><?php echo $gear_tech_1 ?></li>
                <li><?php echo $gear_tech_2 ?></li>
                <li><?php echo $gear_tech_3 ?></li>
                <li><?php echo $gear_tech_4 ?></li>
                <li><?php echo $gear_tech_5 ?></li>
                <li><?php echo $gear_tech_6 ?></li>
              </ul>
            </div>
            <div class="spec left data-management">
              <h2 class="spec-title"><?php echo $gear_data ?></h2>
              <ul class="spec-list">
                <li><?php echo $gear_data_1 ?></li>
                <li><?php echo $gear_data_2 ?></li>
                <li><?php echo $gear_data_3 ?></li>
              </ul>
            </div>
          </div>
          <div class="group">
            <div class="spec right midi-controller">
              <h2 class="spec-title"><?php echo $gear_midi ?></h2>
              <ul class="spec-list">
                <li><?php echo $gear_midi_1 ?></li>
                <li><?php echo $gear_midi_2 ?></li>
              </ul>
            </div>
            <div class="spec right midi-specification">
              <h2 class="spec-title"><?php echo $gear_spec ?></h2>
              <ul class="spec-list">
                <li><?php echo $gear_spec_1 ?></li>
                <li><?php echo $gear_spec_2 ?></li>
                <li><?php echo $gear_spec_3 ?></li>
                <li><?php echo $gear_spec_4 ?></li>
                <li><?php echo $gear_spec_5 ?></li>
                <li><?php echo $gear_spec_6 ?></li>
                <li><?php echo $gear_spec_7 ?></li>
              </ul>
            </div>
          </div>
        </div>
      </div>
    </section>
    <section id="samples">&nbsp;</section>
    <section id="testimonial">
      <h2 class="section-heading"><?php echo $h2_3 ?></h2>
      <p class="section-sub"><?php echo $slogan_3 ?></p>
      <div class="testimonials" lang="en">
        <div class="flexslider">
          <ul class="slides">
            <li>
              <blockquote class="testimonial-text">Your B-step Sequencer 2 is really great - minimal and clean - lovely work.</blockquote>
              <div class="testimonial-image"><img src="img/an_user.jpg" alt="<?php $misc_musician ?> noavatar"></div>
              <p class="details">Felix</p>
            </li>
            <li>
              <blockquote class="testimonial-text">I have installed the new version, and first of all I'm impressed with the stunning look of it !! Good job !!</blockquote>
              <div class="testimonial-image"><img src="img/an_user.jpg" alt="<?php $misc_musician ?> noavatar"></div>
              <p class="details">Luc</p>
            </li>
            <li>
              <blockquote class="testimonial-text">Excellent sequencer, love the ergonomics and some brilliant features that I've only seen in Sonic Bytes' ERA.</blockquote>
              <div class="testimonial-image"><img src="img/an_user.jpg" alt="<?php $misc_musician ?> noavatar"></div>
              <p class="details">Varikus</p>
            </li>
          </ul>
        </div>
        <div class="custom-navigation">
          <div class="custom-controls-container"></div>
        </div>
      </div>
    </section>
    <section id="pricing">
      <h2 class="section-heading"><?php echo $pricing_h ?></h2>
      <p class="section-sub"><?php echo $pricing_slogan ?></p>
      <div class="container">
        <div class="free">
          <div class="inner">
            <h2><?php echo $demo_h ?>*</h2>
            <?php echo $demo_body ?>
            <p class="price"><?php echo $demo_price ?></p><a href="#downloads" class="button monique-button"><span class="fa fa-download"></span><?php echo $demo_dl ?></a>
          </div>
          <p class="cond"><span>*<?php echo $demo_dv ?></p>
        </div>
        <div class="standard">
          <div class="inner">
            <h2><?php echo $pro_h ?></h2>
            <?php echo $pro_body ?>
            <p class="price"><?php echo $pro_price ?></p><a href="http://sites.fastspring.com/monoplugs/product/b-step-sequencer" class="button monique-button"><span class="fa fa-shopping-cart"></span><?php echo $pro_dl ?></a>
          </div>
        </div>
      </div>
    </section>
    <section id="downloads">
      <h2 class="section-heading"><?php echo $download_h ?></h2>
      <div class="point-container group">
        <div class="point download">
          <div><span class="os-logo"><img src="img/windows.png" alt="<?php echo "download ".$alt_windows ?>"></span>
            <h3>Windows</h3>
            <p>VST, Standalone, 32bit, 64Bit. Windows XP+</p>
          </div><a href="http://b-step.monoplugs.com/downloads/windows/B-Step-Sequencer-2-Trial-Setup.zip" onclick="ga('send', 'event', 'Download', 'Win2', 'main');" class="button windows-button"><span class="fa fa-download"></span>Download</a>
        </div>
        <div class="point download">
          <div><span class="os-logo"><img src="img/apple.png" alt="<?php echo "download ".$alt_mac ?>"></span>
            <h3>Mac</h3>
            <p>VST, Standalone, Audio Unit (MIDI FX) , 32bit (Intel), 64Bit (Intel), Mac OS 10.5+</p>
          </div><a href="http://b-step.monoplugs.com/downloads/mac/B-Step-Sequencer-2-Trial-Setup.zip" onclick="ga('send', 'event', 'Download', 'Mac2', 'main');" class="button mac-button"><span class="fa fa-download"></span>Download</a>
        </div>
        <div class="point download">
          <div><span class="os-logo"><img src="img/linux.png" alt="<?php echo "download ".$alt_linux ?>"></span>
            <h3>Linux</h3>
            <p> VST (native), Standalone, 32bit, 64Bit</p>
          </div><a href="http://b-step.monoplugs.com/downloads/linux/B-Step-Sequencer-2-Trial-Installer.zip" onclick="ga('send', 'event', 'Download', 'Linux2', 'main');" class="button linux-button"><span class="fa fa-download"></span>Download</a>
        </div>
        <div class="point download">
          <div><span class="os-logo"><img src="img/rasberry.png" alt="<?php echo "download ".$alt_raspberry ?>"></span>
            <h3>RASPBERRY PI</h3>
            <p>Standalone, Pi V1+</p>
          </div><a href="http://b-step.monoplugs.com/downloads/raspberry/B-Step-Sequencer-2-Trial.zip" onclick="ga('send', 'event', 'Download', 'Raspberry', 'main');" class="button rasberry-button"><span class="fa fa-download"></span>Download</a>
        </div>
        <div class="point download">
          <div><span class="os-logo"><img src="img/ios.png" alt="<?php echo "download ".$alt_ios ?>"></span>
            <h3>iPad</h3>
            <p>iOS 7.0+</p>
          </div><a href="https://itunes.apple.com/app/id855085232" onclick="ga('send', 'event', 'Download', 'iPad2', 'main');" class="button ios-button"><img src="img/app_store.png" alt="Download on the AppStore"></a>
        </div>
        <div class="point download">
          <div><span class="os-logo"><img src="img/android.png" alt="<?php echo "download ".$alt_android ?>"></span>
            <h3>Android</h3>
            <p>Display > 7'', OS 3.2+</p>
          </div><a href="https://play.google.com/store/apps/details?id=com.monoplugs.trial" onclick="ga('send', 'event', 'Download', 'Android2', 'main');" class="button android-button"><img src="img/gplay.png" alt="Download on Google Play"></a>
        </div>
      </div>
    </section>
    <section id="about">
      <div class="about container">
      </div>
    </section>
    <footer id="site-footer">
      <div class="container group">
        <nav class="site-nav group"><a href="?lang=<?php echo $lang ?>" class="brand">B-Step Sequencer</a>
          <ul>
            <li><a href="#site-hero"><?php echo $top ?></a></li>
            <li><a href="#highlights"><?php echo $highlights ?></a></li>
            <li><a href="#pricing"><?php echo $pricing ?></a></li>
            <!--<li><a href="#downloads"><?php echo $dl_link ?></a></li>-->
            <li><a href="/manual"><?php echo $manual_en ?> ></a></li>
            <li><a href="http://monoplugs.com/magazine/category/b-step-sequencer/"><?php echo $magazine ?> ></a></li>
          </ul>
        </nav>
        <div class="footer-extra group">
          <div class="pull-left">
            <p><?php echo $slogan ?></p><a href="http://Monoplugs.com"><img src="img/MonoPlugs.png" alt="Monoplugs Logo" class="brand-logo"></a>
          </div>
          <div class="pull-right">
            <p><a href="http://monoplugs.com/#contact"><span class="fa fa-envelope"></span><?php echo $contact ?></a><a href="http://monoplugs.com"><span class="fa fa-globe"></span>Monoplugs.com</a><a href="http://monoplugs.com/forum" target="_blank"><span class="fa fa-support"></span>Forum</a></p>
            <p class="social-links"><a href="https://www.facebook.com/Monoplugs" class="fa fa-facebook-official"></a><a href="https://twitter.com/monotomys" class="fa fa-twitter-square"></a><a href="https://www.youtube.com/channel/UCEf3A0_9I-JE8xYXrxUBW-A" class="fa fa-youtube-square"></a></p>
          </div>
        </div>
      </div>
    </footer>
    
    <link href="http://vjs.zencdn.net/5.6.0/video-js.css" rel="stylesheet">
    <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/font-awesome/4.5.0/css/font-awesome.min.css" type="text/css">
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/1.8.3/jquery.min.js" type="text/javascript"></script>
    <script src="js/jquery.flexslider-min.js" type="text/javascript"></script>
    <script src="http://vjs.zencdn.net/ie8/1.1.2/videojs-ie8.min.js"></script>
    <script type="text/javascript" charset="utf-8">
      $(window).load(function() {
      $('.flexslider').flexslider({
      animation: "slide",
      touch: true, 
      directionNav: false, 
      
      controlsContainer: $(".custom-controls-container"),
      customDirectionNav: $(".custom-navigation a")
      });
      });
    </script>
    <script src="http://vjs.zencdn.net/5.6.0/video.js"></script>
    
    <script type="text/javascript">(function(i,s,o,g,r,a,m){i['GoogleAnalyticsObject']=r;i[r]=i[r]||function(){(i[r].q=i[r].q||[]).push(arguments)},i[r].l=1*new Date();a=s.createElement(o),m=s.getElementsByTagName(o)[0];a.async=1;a.src=g;m.parentNode.insertBefore(a,m)})(window,document,'script','//www.google-analytics.com/analytics.js','ga');ga('create','UA-47075200-1','auto');ga('send','pageview');</script>
    
    <script src="js/audioplayer.js" type="text/javascript"></script>
    <script src="js/functions.js" type="text/javascript"></script>

    <script type="application/ld+json">
    {
    "@context": "http://schema.org/",
    "@type": "SoftwareApplication",
    "name": "B-Step Sequencer",
    "description" : "The harmonic music step sequencer plugin to create your melodies, arpeggios and bass lines on the fly, now everyone can be pro.",
    "operatingSystem": "Windows XP, Mac OS 10.5, Linux, iOS 7, Android",
    "applicationCategory": "http://schema.org/MusicInstrument",
    "applicationSubCategory": "Step Sequencer",
    "softwareVersion": "2.1",
    "datePublished" : "2015-02-19",
    "alternativeHeadline": "Arpeggios and Melodies, created in Seconds",
    "softwareHelp": "http://b-step.monoplugs.com/manual",
    "screenshot": "http://b-step.monoplugs.com/img/b-step-sequencer.jpg",
    
    "copyrightHolder": "Monoplugs",
    "copyrightYear": "2016",
    
    "keywords": "Sequencer, VST Plugin, Sequencer Plugin, Virtual Sequencer",
    
    "sameAs" : 
    [ 
    "https://www.facebook.com/b.step.sequencer",
    "http://www.kvraudio.com/product/b-step-sequencer-by-monoplugs",
    "https://en.wikipedia.org/wiki/B-Step_Sequencer",
    "http://sites.fastspring.com/monoplugs/product/b-step-sequencer"
    ],
    
    "typicalAgeRange":"16-60",
    
    "offers": 
    {
        "@type": "Offer",
        "price": "49.00",
        "priceCurrency": "USD"
    },
    
    "aggregateRating": 
    {
        "@type": "AggregateRating",
        "ratingValue": "4,5",
        "ratingCount": "10"
    },
    
    "video": 
    {
        "@type": "VideoObject",
        "contentUrl": "https://youtu.be/SPEdBFOirkg",
        "description": "A tutorial how to create an arpeggio with the software step sequencer: B-Step",
        "name": "B-Step Sequencer - Create a swinging Arpeggio",
        "thumbnailUrl": "https://i.ytimg.com/vi/SPEdBFOirkg/default.jpg",
        "uploadDate": "2014-06-11"
    }

    }
    </script>
    
    <link rel="preconnect" href="http://monoplugs.com">
    
  </body>
</html>

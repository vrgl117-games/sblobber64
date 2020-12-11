#!/usr/bin/env bash

fontb='resources/fonts/Kenney Blocks.ttf'

#UI
#-extent XXxYY
convert -strip -background "#222323ff" -fill "#5dde87ff" -font "$fontb" -pointsize 68 label:"sblobber 64" -extent 518x128 resources/gfx/sprites/ui/logo.png
convert -strip resources/gfx/sprites/ui/logo.png -crop 16 resources/gfx/sprites_sets/ui/logo-%d_0.png
convert -strip -background "#222323ff" -fill "#f2a65eff" -font "$fontb" -pointsize 28 label:"\ Take the stairs to\nfind your friend" -extent 344x128 -crop 16 resources/gfx/sprites_sets/ui/take_the_stairs-%d_0.png
convert -strip -background "#222323ff" -fill "#5dde87ff" -font "$fontb" -pointsize 68 label:"Pause" -extent 256x104 resources/gfx/sprites/ui/pause.png
convert -strip -background "#222323ff" -fill "#eb554bff" -font "$fontb" -pointsize 68 label:"Game Over !" -extent 494x104 resources/gfx/sprites/ui/game_over.png
convert -strip -background "#222323ff" -fill "#5dde87ff" -font "$fontb" -pointsize 68 label:"Credits" -extent 358x104 resources/gfx/sprites/ui/credits_big.png
convert -strip -background "#222323ff" -fill "#5dde87ff" -font "$fontb" -pointsize 28 label:"You burned in the fire" -extent 408x44 resources/gfx/sprites/ui/fire.png
convert -strip -background "#222323ff" -fill "#5dde87ff" -font "$fontb" -pointsize 28 label:"You fell through the grid" -extent 470x44 resources/gfx/sprites/ui/grid.png
convert -strip -background "#222323ff" -fill "#5dde87ff" -font "$fontb" -pointsize 28 label:"Resume" -extent 134x44 resources/gfx/sprites/ui/resume.png
convert -strip -background "#222323ff" -fill "#ffffffff" -font "$fontb" -pointsize 24 label:"Continue" -extent 146x38 resources/gfx/sprites/ui/continue.png
convert -strip -background "#222323ff" -fill "#5dde87ff" -font "$fontb" -pointsize 28 label:"Restart" -extent 148x44 resources/gfx/sprites/ui/restart.png
convert -strip -background "#222323ff" -fill "#5dde87ff" -font "$fontb" -pointsize 28 label:"Credits" -extent 148x44 resources/gfx/sprites/ui/credits.png
convert -strip -background "#222323ff" -fill "#5dde87ff" -font "$fontb" -pointsize 28 label:"Quit" -extent 86x44 resources/gfx/sprites/ui/quit.png
convert -strip -background "#222323ff" -fill "#ffffffff" -font "$fontb" -pointsize 28 label:"Resume" -extent 134x44 resources/gfx/sprites/ui/resume_selected.png
convert -strip -background "#222323ff" -fill "#ffffffff" -font "$fontb" -pointsize 28 label:"Restart" -extent 148x44 resources/gfx/sprites/ui/restart_selected.png
convert -strip -background "#222323ff" -fill "#ffffffff" -font "$fontb" -pointsize 28 label:"Credits" -extent 148x44 resources/gfx/sprites/ui/credits_selected.png
convert -strip -background "#222323ff" -fill "#ffffffff" -font "$fontb" -pointsize 28 label:"Quit" -extent 86x44 resources/gfx/sprites/ui/quit_selected.png
convert -strip -background "#222323ff" -fill "#5dde87ff" -font "$fontb" -pointsize 36 -gravity center label:"Well done,\nyou found your friend!" -extent 532x110  resources/gfx/sprites/ui/you_win.png
convert -strip -background "#222323ff" -fill "#f2a65eff" -font "$fontb" -pointsize 28 label:"Thanks for playing!" -extent 366x44 resources/gfx/sprites/ui/thanks.png
convert -strip -background "#222323ff" -fill "#5dde87ff" -font "$fontb" -pointsize 28 label:"Programming" -extent 246x44 resources/gfx/sprites/ui/programming.png
convert -strip -background "#222323ff" -fill "#ffffffff" -font "$fontb" -pointsize 24 label:"Isabel Jimenez & Victor Vieux" -extent 482x38 resources/gfx/sprites/ui/isabel_victor.png
convert -strip -background "#222323ff" -fill "#ffffffff" -font "$fontb" -pointsize 24 label:"www.vrgl117.games" -extent 320x38  resources/gfx/sprites/ui/vrgl117games.png
convert -strip -background "#222323ff" -fill "#5dde87ff" -font "$fontb" -pointsize 28 label:"Art" -extent 64x44 resources/gfx/sprites/ui/art.png
convert -strip -background "#222323ff" -fill "#ffffffff" -font "$fontb" -pointsize 24 label:"Kenney" -extent 110x38 resources/gfx/sprites/ui/kenney.png
convert -strip -background "#222323ff" -fill "#ffffffff" -font "$fontb" -pointsize 24 label:"www.kenney.nl" -extent 242x38 resources/gfx/sprites/ui/kenneynl.png
convert -strip -background "#222323ff" -fill "#5dde87ff" -font "$fontb" -pointsize 28 label:"Music" -extent 114x44 resources/gfx/sprites/ui/music.png
#edited by hand convert -strip -background "#222323ff" -fill "#ffffffff" -font "$fontb" -pointsize 24 label:"m0d" -extent 62x38 resources/gfx/sprites/ui/mod.png
convert -strip -background "#222323ff" -fill "#ffffffff" -font "$fontb" -pointsize 24 label:"www.modarchive.org" -extent 338x38 resources/gfx/sprites/ui/modarchive.png
convert -strip -background "#222323ff" -fill "#5dde87ff" -font "$fontb" -pointsize 24 -gravity center label:"Expansion Pak detected" -extent 374x38 resources/gfx/sprites/ui/pak_detected.png
convert -strip -background "#222323ff" -fill "#f2a65eff" -font "$fontb" -pointsize 24 -gravity center label:"Expansion Pak not detected!" -extent 446x38 resources/gfx/sprites/ui/pak_not_detected.png
convert -strip -background "#222323ff" -fill "#5dde87ff" -font "$fontb" -pointsize 24 -gravity center label:"This game is Rumble Pak compatible\n\nPlease insert your Rumble Pak now\nif you wish to play with it" -extent 572x146 resources/gfx/sprites/ui/rumble.png

# studio logo
convert -strip -brightness-contrast -10 resources/gfx/sprites/intro/vrgl117_logo.png resources/gfx/sprites/intro/vrgl117_logo_9.png
convert -strip -brightness-contrast -20 resources/gfx/sprites/intro/vrgl117_logo.png resources/gfx/sprites/intro/vrgl117_logo_8.png
convert -strip -brightness-contrast -30 resources/gfx/sprites/intro/vrgl117_logo.png resources/gfx/sprites/intro/vrgl117_logo_7.png
convert -strip -brightness-contrast -40 resources/gfx/sprites/intro/vrgl117_logo.png resources/gfx/sprites/intro/vrgl117_logo_6.png
convert -strip -brightness-contrast -50 resources/gfx/sprites/intro/vrgl117_logo.png resources/gfx/sprites/intro/vrgl117_logo_5.png
convert -strip -brightness-contrast -60 resources/gfx/sprites/intro/vrgl117_logo.png resources/gfx/sprites/intro/vrgl117_logo_4.png
convert -strip -brightness-contrast -70 resources/gfx/sprites/intro/vrgl117_logo.png resources/gfx/sprites/intro/vrgl117_logo_3.png
convert -strip -brightness-contrast -80 resources/gfx/sprites/intro/vrgl117_logo.png resources/gfx/sprites/intro/vrgl117_logo_2.png
convert -strip -brightness-contrast -90 resources/gfx/sprites/intro/vrgl117_logo.png resources/gfx/sprites/intro/vrgl117_logo_1.png

# n64 + n64brew jam logo
convert -strip -brightness-contrast -10 resources/gfx/sprites/intro/n64brew_jam_logo.png resources/gfx/sprites/intro/n64brew_jam_logo_9.png
convert -strip -brightness-contrast -20 resources/gfx/sprites/intro/n64brew_jam_logo.png resources/gfx/sprites/intro/n64brew_jam_logo_8.png
convert -strip -brightness-contrast -30 resources/gfx/sprites/intro/n64brew_jam_logo.png resources/gfx/sprites/intro/n64brew_jam_logo_7.png
convert -strip -brightness-contrast -40 resources/gfx/sprites/intro/n64brew_jam_logo.png resources/gfx/sprites/intro/n64brew_jam_logo_6.png
convert -strip -brightness-contrast -50 resources/gfx/sprites/intro/n64brew_jam_logo.png resources/gfx/sprites/intro/n64brew_jam_logo_5.png
convert -strip -brightness-contrast -60 resources/gfx/sprites/intro/n64brew_jam_logo.png resources/gfx/sprites/intro/n64brew_jam_logo_4.png
convert -strip -brightness-contrast -70 resources/gfx/sprites/intro/n64brew_jam_logo.png resources/gfx/sprites/intro/n64brew_jam_logo_3.png
convert -strip -brightness-contrast -80 resources/gfx/sprites/intro/n64brew_jam_logo.png resources/gfx/sprites/intro/n64brew_jam_logo_2.png
convert -strip -brightness-contrast -90 resources/gfx/sprites/intro/n64brew_jam_logo.png resources/gfx/sprites/intro/n64brew_jam_logo_1.png
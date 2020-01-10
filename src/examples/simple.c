/* vim: set ts=4 sw=4 wm=5 : */

#include <gtk/gtk.h>
#include <gtkspell/gtkspell.h>

int
main (int argc, char* argv[])
{
  GtkWidget *win, *box, *scroll, *view;
  GError *error = NULL;
  GtkSpellChecker* spell;
  char *errortext = NULL;

  gtk_init (&argc, &argv);

  view = gtk_text_view_new ();
  gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (view), GTK_WRAP_WORD);

  spell = gtk_spell_checker_new ();
  if (!gtk_spell_checker_set_language (spell, "en_US", &error))
    {
      g_print ("gtkspell error: %s\n", error->message);
      errortext = g_strdup_printf ("GtkSpell was unable to initialize.\n"
                                   "%s", error->message);
      g_error_free (error);
    }
  gtk_spell_checker_attach (spell, GTK_TEXT_VIEW (view));

  scroll = gtk_scrolled_window_new (NULL, NULL);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scroll),
                                  GTK_POLICY_AUTOMATIC,
                                  GTK_POLICY_AUTOMATIC);
  gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scroll),
                                       GTK_SHADOW_IN);
  gtk_container_add (GTK_CONTAINER (scroll), view);
#if GTK_MAJOR_VERSION == 2
  box = gtk_vbox_new (FALSE, 5);
#elif GTK_MAJOR_VERSION == 3
  box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 5);
#else
  #error Unsupported GTK version
#endif
  if (errortext)
    {
      gtk_box_pack_start (GTK_BOX (box), gtk_label_new (errortext),
                          FALSE, FALSE, 0);
      g_free (errortext);
    }
  else
    {
      gtk_box_pack_start (GTK_BOX (box),
                          gtk_label_new ("Type some text into the text box.\n"
                     "Try misspelling some words.  Then right-click on them."),
                          FALSE, FALSE, 0);
    }
  gtk_box_pack_start (GTK_BOX (box), scroll, TRUE, TRUE, 0);
  gtk_widget_show_all (box);

  win = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size (GTK_WINDOW (win), 400, 300);
  gtk_window_set_title (GTK_WINDOW (win), "Simple GtkSpell Demonstration");
  gtk_container_set_border_width (GTK_CONTAINER (win), 10);
  g_signal_connect (win, "delete-event", G_CALLBACK (gtk_main_quit), NULL);
  gtk_container_add (GTK_CONTAINER (win), box);

  gtk_widget_show (win);
  gtk_main ();

  return 0;
}

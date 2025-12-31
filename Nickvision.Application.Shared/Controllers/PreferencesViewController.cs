using Nickvision.Application.Shared.Models;
using Nickvision.Desktop.Application;
using Nickvision.Desktop.Filesystem;
using Nickvision.Desktop.Globalization;
using System.Collections.Generic;
using System.Threading.Tasks;

namespace Nickvision.Application.Shared.Controllers;

public class PreferencesViewController
{
    private readonly IJsonFileService _jsonFileService;
    private readonly Configuration _configuration;

    public ITranslationService Translator { get; }

    public PreferencesViewController(IJsonFileService jsonFileService, ITranslationService translationService)
    {
        _jsonFileService = jsonFileService;
        _configuration = _jsonFileService.Load<Configuration>(Configuration.Key);
        Translator = translationService;
    }

    public IReadOnlyList<SelectionItem<Theme>> Themes
    {
        get => new List<SelectionItem<Theme>>()
        {
            new SelectionItem<Theme>(Models.Theme.Light, Translator._p("Theme", "Light"), _configuration.Theme == Models.Theme.Light),
            new SelectionItem<Theme>(Models.Theme.Dark, Translator._p("Theme", "Dark"), _configuration.Theme == Models.Theme.Dark),
            new SelectionItem<Theme>(Models.Theme.System, Translator._p("Theme", "System"), _configuration.Theme == Models.Theme.System),
        };
    }

    public SelectionItem<Theme> Theme
    {
        set => _configuration.Theme = value.Value;
    }

    public IReadOnlyList<SelectionItem<string>> AvailableTranslationLanguages
    {
        get
        {
            var availableLanguages = new List<SelectionItem<string>>()
            {
                new SelectionItem<string>(string.Empty, Translator._("System"), string.IsNullOrEmpty(_configuration.TranslationLanguage)),
                new SelectionItem<string>("C", "en_US", _configuration.TranslationLanguage == "C")
            };
            foreach (var language in Translator.AvailableLanguages)
            {
                availableLanguages.Add(new SelectionItem<string>(language, language, _configuration.TranslationLanguage == language));
            }
            return availableLanguages;
        }
    }

    public SelectionItem<string> TranslationLanguage
    {
        set => _configuration.TranslationLanguage = value.Value;
    }

    public bool AllowPreviewUpdates
    {
        get => _configuration.AllowPreviewUpdates;

        set => _configuration.AllowPreviewUpdates = value;
    }

    public async Task SaveConfigurationAsync() => await _jsonFileService.SaveAsync(_configuration, Configuration.Key);
}
